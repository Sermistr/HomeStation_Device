/**
 * @file Telegram_Task.cpp
 * @brief FreeRTOS task for handling Telegram bot interactions.
 *
 * @details
 * This module provides the interface for managing Telegram bot communications.
 * The task periodically checks for new messages and responds with sensor data.
 *
 * @author  Sergei Lazarev
 * @date    2026-02-17
 * @version 1.0
 *
 * @note    Designed for ESP32 platform running FreeRTOS.
 */


#include "Telegram_Task.h"

WiFiClientSecure client;
UniversalTelegramBot bot(WiFiConfig::BOT_TOKEN, client);
String keyboard_menu  = "[[\"Temperature\", \"Humidity\"]]";


float lastTemperature = 0;
float lastHumidity = 0;
bool lastWiFiStatus = false;

void TelegramTask(void *pvParameters) 
{
    client.setInsecure();
    vTaskDelay(pdMS_TO_TICKS(5000));  // wait WiFi

    bot.getUpdates(bot.last_message_received + 1); 

    for(;;) 
    {
        if (xSemaphoreTake(dataMutex, MUTEX_TIMEOUT) == pdTRUE) 
        {
            lastWiFiStatus = sensorData.wifiConnected;
            xSemaphoreGive(dataMutex);
        }
        
        if (lastWiFiStatus) 
        {
            if (xSemaphoreTake(dataMutex, MUTEX_TIMEOUT) == pdTRUE) 
            {
                lastTemperature = sensorData.temperature;
                lastHumidity = sensorData.humidity;
                xSemaphoreGive(dataMutex);
            }

            HandleNewMessages(bot.getUpdates(bot.last_message_received + 1));
        }
        
        vTaskDelay(TELEGRAM_TASK_PERIOD_MS / portTICK_PERIOD_MS);
    }
}

void HandleNewMessages(int numNewMessages) 
{
    for (int i = 0; i < numNewMessages; i++) 
    {
        String chat_id = String(bot.messages[i].chat_id);
        //String text = bot.messages[i].text;
        if (bot.messages[i].text == "/start") 
        {
            if (chat_id == WiFiConfig::ADMIN_CHAT_ID)
            {
                String welcomeMessage = "Welcome Home, Sir!\n";
                welcomeMessage += "You know all what to do!\n";
                bot.sendMessage(chat_id, welcomeMessage, "");
            }
            else
            {
                String welcomeMessage = "Welcome to the Home Sensor Bot!\n";
                welcomeMessage += "If you want to get current sensor readings, use the command /status.\n";
                bot.sendMessage(chat_id, welcomeMessage, "");
            }

        }
        else if (bot.messages[i].text == "/status") 
        {
            String message = "Choose a parameter to get the current reading:";
            bot.sendMessageWithReplyKeyboard(chat_id, message, "", keyboard_menu, true, true);
        }
        else if (bot.messages[i].text == "Temperature") 
        {
            String message = "Current Temperature: " + String(lastTemperature, 2) + " °C";
            bot.sendMessage(chat_id, message, "");
        } 
        else if (bot.messages[i].text == "Humidity") 
        {
            String message = "Current Humidity: " + String(lastHumidity, 2) + " %";
            bot.sendMessage(chat_id, message, "");
        }
        else if (bot.messages[i].text == "Exit") 
        {
            if (chat_id == WiFiConfig::ADMIN_CHAT_ID)
            {
                String welcomeMessage = "All the best, Sir!\n";
                bot.sendMessage(chat_id, welcomeMessage, "");
            }
            else
            {
                String message = "Thank you for using the Home Sensor Bot! If you want to use this bot again, just say /start.";
                bot.sendMessage(chat_id, message, "");
            }

        }
        else if(bot.messages[i].text == "/reload")
        {
            if (chat_id == WiFiConfig::ADMIN_CHAT_ID)
            {
                String message = "Reloading the system...";
                bot.sendMessage(chat_id, message, "");
                WiFi.disconnect();
                vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait for 2 seconds before restarting
                
                if (xSemaphoreTake(dataMutex, MUTEX_TIMEOUT) == pdTRUE) 
                {
                    sensorData.wifiConnected = false; // Update WiFi status to false before restarting
                    xSemaphoreGive(dataMutex);
                }

                SystemEvent_t event = {
                    .name = WIFI,
                    .status = SYSTEM_WARN
                };
                xQueueSend(systemEventQueue, &event, QUEUE_TIMEOUT);
                
                if (ESP.getFreeHeap() > 1024) {
                    ESP.restart();
                } else {
                    String errorMessage = "Not enough memory to restart. Please try again later.";
                    bot.sendMessage(chat_id, errorMessage, "");
                }

            }
            else
            {
                String message = "You don't have permission to perform this action.";
                bot.sendMessage(chat_id, message, "");
            }
        }
        else 
        {
            String message = "Unknown command. Please use /status to get current sensor readings.";
            bot.sendMessage(chat_id, message, "");
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Short delay to avoid flooding
    }
}