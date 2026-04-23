/**
 * @file GPT_Task.cpp
 * @brief FreeRTOS task that asks a Groq-hosted LLM for outfit / activity
 *        advice based on the current DHT11 temperature and humidity readings.
 *
 * @details
 * The task waits for Wi-Fi to come up, then on each cycle:
 *   1. Copies the latest temperature / humidity out of the shared
 *      sensorData struct (under mutex protection).
 *   2. Skips the network round-trip when neither value has changed by
 *      at least GPT_REQUEST_DELTA since the last successful reply.
 *   3. POSTs a chat-completions request to the Groq API (OpenAI-compatible).
 *   4. Parses the reply with ArduinoJson and copies the advice text
 *      back into sensorData.gptAdvice so that other tasks (Telegram,
 *      LCD, ...) can surface it to the user.
 *   5. Publishes a SystemEvent_t so that SystemCheckTask keeps the
 *      status LED in sync with the GPT subsystem health.
 *
 * @author  Sergei Lazarev
 * @date    2026-04-23
 * @version 1.2
 * @note    Designed for ESP32 platform running FreeRTOS.
 */

#include "GPT_Task.h"

// Groq chat-completions endpoint (OpenAI-compatible).
static const char *GROQ_ENDPOINT = "https://api.groq.com/openai/v1/chat/completions";

// Last values we actually sent. Used to throttle requests when the readings
// have not meaningfully changed.
static float    lastSentTemperature = NAN;
static float    lastSentHumidity    = NAN;
static int16_t  lastSentLight       = -1;

// A change of this many raw ADC counts (~5 %) counts as "meaningful".
#define PHOTO_DELTA_COUNTS 200


static void PublishGptStatus(System_Status status)
{
    SystemEvent_t evt = {
        .name = GPT_CHAT,
        .status = status
    };
    xQueueSend(systemEventQueue, &evt, QUEUE_TIMEOUT);
}


static void StoreAdvice(const char *advice)
{
    if (xSemaphoreTake(dataMutex, MUTEX_TIMEOUT) == pdTRUE)
    {
        strlcpy(sensorData.gptAdvice, advice, GPT_ADVICE_MAX_LEN);
        xSemaphoreGive(dataMutex);
    }
}


void GPTTask(void *pvParameters)
{
    // Wait a bit so Wi-Fi has a chance to come up before the first request.
    vTaskDelay(pdMS_TO_TICKS(8000));

    float    temperature = 0.0f;
    float    humidity    = 0.0f;
    uint16_t light       = 0;
    bool     wifiReady   = false;

    for (;;)
    {
        if (xSemaphoreTake(dataMutex, MUTEX_TIMEOUT) == pdTRUE)
        {
            temperature = sensorData.temperature;
            humidity    = sensorData.humidity;
            light       = sensorData.lightLevel;
            wifiReady   = sensorData.wifiConnected;
            xSemaphoreGive(dataMutex);
        }

        if (!wifiReady)
        {
            PublishGptStatus(SYSTEM_WARN);
            vTaskDelay(pdMS_TO_TICKS(GPT_TASK_PERIOD_MS));
            continue;
        }

        // Skip useless calls while DHT is still warming up or reporting zeroes.
        if (temperature == 0.0f && humidity == 0.0f)
        {
            vTaskDelay(pdMS_TO_TICKS(GPT_TASK_PERIOD_MS));
            continue;
        }

        // Skip the request when nothing has changed meaningfully.
        bool firstRequest = isnan(lastSentTemperature) || isnan(lastSentHumidity)
                         || lastSentLight < 0;
        bool meaningfulMove = firstRequest
            || fabsf(temperature - lastSentTemperature) >= GPT_REQUEST_DELTA
            || fabsf(humidity    - lastSentHumidity)    >= GPT_REQUEST_DELTA
            || abs((int)light - lastSentLight)          >= PHOTO_DELTA_COUNTS;

        if (!meaningfulMove)
        {
            vTaskDelay(pdMS_TO_TICKS(GPT_TASK_PERIOD_MS));
            continue;
        }

        if (RequestGptAdvice(temperature, humidity, light))
        {
            lastSentTemperature = temperature;
            lastSentHumidity    = humidity;
            lastSentLight       = (int16_t)light;
            PublishGptStatus(SYSTEM_OK);
        }
        else
        {
            PublishGptStatus(SYSTEM_ERROR);
        }

        vTaskDelay(pdMS_TO_TICKS(GPT_TASK_PERIOD_MS));
    }
}


static void DumpResponse(const String &response)
{
    Serial.printf("[GPT] raw response (len=%u):\n", response.length());
    const size_t chunk = 200;
    for (size_t i = 0; i < response.length(); i += chunk)
    {
        Serial.println(response.substring(i, i + chunk));
    }
}


bool RequestGptAdvice(float temperature, float humidity, uint16_t lightRaw)
{
    WiFiClientSecure client;
    client.setInsecure(); // matches the approach used by the Telegram task
    client.setTimeout(15); // seconds

    HTTPClient https;
    https.setTimeout(15000);
    https.useHTTP10(true); // force HTTP/1.0 -> no chunked transfer encoding
    if (!https.begin(client, GROQ_ENDPOINT))
    {
        Serial.println("[GPT] https.begin() failed");
        return false;
    }

    https.addHeader("Content-Type", "application/json");
    https.addHeader("Authorization", String("Bearer ") + WiFiConfig::GROQ_API_KEY);

    // Build the chat-completions payload. Keep the prompt short to save tokens.
    // High temperature + presence_penalty push the model towards more varied
    // and less repetitive pop-culture references between requests.
    StaticJsonDocument<512> payload;
    payload["model"] = WiFiConfig::GROQ_MODEL;
    payload["temperature"]      = 1.05;
    payload["top_p"]            = 0.95;
    payload["presence_penalty"] = 0.6;
    payload["frequency_penalty"]= 0.3;
    payload["max_tokens"]       = 220;

    JsonArray messages = payload.createNestedArray("messages");

    JsonObject sys = messages.createNestedObject();
    sys["role"] = "system";
    sys["content"] =
        "You are a witty home-climate advisor with a flair for pop culture. "
        "Given indoor temperature (Celsius), relative humidity (%) and "
        "ambient light (0 % = pitch dark, 100 % = bright daylight), reply "
        "in 2-3 short, original sentences with practical advice about "
        "what to wear and what to do right now — always weaving in a "
        "reference to a film, cartoon, anime, video game or comic book "
        "that fits the vibe (e.g. 'worthy of Mordor', 'the Batcave is "
        "brighter than this', 'channel your inner Totoro', 'Dune-level "
        "dryness'). Vary the reference every time and pick something "
        "unexpected; do not repeat yourself. Keep the tone clever and "
        "polite — NOT chummy, NOT using slang like 'bro', 'dude', "
        "'братишка' or 'дружище'. Address the user neutrally or with "
        "'you / вы'. Reply in the same language the user wrote in.";

    int lightPct = (int)((lightRaw * 100UL) / 4095UL);
    char userMsg[192];
    snprintf(userMsg, sizeof(userMsg),
             "Дома сейчас %.1f°C, влажность %.0f%%, освещённость %d%%. "
             "Что посоветуешь надеть или сделать?",
             temperature, humidity, lightPct);

    JsonObject usr = messages.createNestedObject();
    usr["role"] = "user";
    usr["content"] = userMsg;

    String body;
    serializeJson(payload, body);

    int httpCode = https.POST(body);
    if (httpCode != HTTP_CODE_OK)
    {
        Serial.printf("[GPT] POST failed, code=%d\n", httpCode);
        if (httpCode > 0)
        {
            Serial.println(https.getString());
        }
        https.end();
        return false;
    }

    String response = https.getString();
    https.end();

    DynamicJsonDocument reply(8192);
    DeserializationError err = deserializeJson(reply, response);
    if (err)
    {
        Serial.printf("[GPT] JSON parse error: %s\n", err.c_str());
        DumpResponse(response);
        return false;
    }

    JsonVariant contentVar = reply["choices"][0]["message"]["content"];
    if (contentVar.isNull() || !contentVar.is<const char *>())
    {
        const char *finish = reply["choices"][0]["finish_reason"] | "unknown";
        Serial.printf("[GPT] No 'content' (finish_reason=%s, isNull=%d, isString=%d)\n",
                      finish,
                      (int)contentVar.isNull(),
                      (int)contentVar.is<const char *>());
        DumpResponse(response);
        return false;
    }

    String advice = contentVar.as<String>();
    advice.trim();
    if (advice.length() == 0)
    {
        Serial.println("[GPT] empty content");
        DumpResponse(response);
        return false;
    }

    StoreAdvice(advice.c_str());
    //Serial.print("[GPT] advice: ");
    //Serial.println(advice);
    return true;
}
