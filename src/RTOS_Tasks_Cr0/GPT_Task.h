/**
 * @file GPT_Task.h
 * @brief FreeRTOS task interface for OpenAI ChatGPT climate-advice requests.
 *
 * @details
 * This header defines the interface for the GPT task, which periodically
 * takes the most recent DHT11 temperature and humidity readings from the
 * shared sensorData structure, sends them to the OpenAI Chat Completions
 * API, and stores the returned natural-language recommendation (what to
 * wear, what to do given the current climate) back into sensorData so
 * that other tasks (e.g. Telegram) can deliver it to the user.
 *
 * @author  Sergei Lazarev
 * @date    2026-04-23
 * @version 1.0
 * @note    Designed for ESP32 platform running FreeRTOS.
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#if __has_include("WiFiConfig.h")
    #include "WiFiConfig.h"
#else
    #include "WificonfigTemplate.h"
    #warning "WiFiConfig.h not found. Using template values."
#endif

#include "sharedData.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

/* Macros --------------------------------------------------------------------*/

/**
 * @brief GPT task execution period in milliseconds.
 *
 * The OpenAI API is billed per request, so we keep the poll interval
 * comfortably long (60 seconds by default). The DHT11 only resolves 1 %RH
 * and 1 °C so more frequent requests would add cost without new insight.
 */
#define GPT_TASK_PERIOD_MS 60000U

/**
 * @brief Minimum absolute change (°C or %RH) that triggers a fresh request.
 *
 * If neither temperature nor humidity has changed by at least this much
 * since the last successful reply, the task skips the API round-trip and
 * keeps the previous advice.
 */
#define GPT_REQUEST_DELTA  1.0f

/* Public Function Prototypes ------------------------------------------------*/

/**
 * @brief FreeRTOS task that asks ChatGPT for climate-based advice.
 *
 * @param pvParameters Pointer to task parameters (unused).
 */
void GPTTask(void *pvParameters);

/**
 * @brief Sends a single climate sample to the LLM and stores the reply.
 *
 * @param temperature Current temperature in degrees Celsius.
 * @param humidity    Current relative humidity in percent.
 * @param lightRaw    Current raw ADC reading of the photo-sensor (0..4095).
 * @return true on HTTP 200 with a parseable reply, false otherwise.
 */
bool RequestGptAdvice(float temperature, float humidity, uint16_t lightRaw);
