/**
 * @file Wifi_Task.cpp
 * @brief Implementation of Wi-Fi management task in FreeRTOS.
 * This file contains the implementation of the Wi-Fi task, which is responsible for
 * managing the Wi-Fi connection, monitoring its status, and communicating events to the system event queue.  
 * The task periodically checks the Wi-Fi connection status and attempts to reconnect if disconnected.
 *
 * @author Serghei
 * @date 2024-06-01
 */
#include "Wifi_Task.h"

uint8_t counter = 0;


// WiFi Management Task
void WiFiTask(void *pv)
{
    WiFi.begin(WiFiConfig::HOME_SSID, WiFiConfig::HOME_PASSWORD);

    bool prevState = false;

    for(;;)
    {
        wl_status_t status = WiFi.status();
        bool currentState = (status == WL_CONNECTED);


        if (status == WL_NO_SSID_AVAIL)
        { 
            if (xSemaphoreTake(dataMutex, MUTEX_TIMEOUT) == pdTRUE)
            {
                sensorData.wifiConnected = false;
                xSemaphoreGive(dataMutex);
            }
            SystemEvent_t evt;
            evt.name = WIFI;
            evt.status = SYSTEM_WARN;
            xQueueSend(systemEventQueue, &evt, QUEUE_TIMEOUT);
        }

        if (currentState != prevState)
        {
            if (xSemaphoreTake(dataMutex, MUTEX_TIMEOUT) == pdTRUE)
            {
                sensorData.wifiConnected = currentState;
                xSemaphoreGive(dataMutex);
            }

            SystemEvent_t evt;
            evt.name = WIFI;
            evt.status = currentState ? SYSTEM_OK : SYSTEM_ERROR;

            xQueueSend(systemEventQueue, &evt, QUEUE_TIMEOUT);

            prevState = currentState;
        }

        if (!currentState)
        {
            WiFi.reconnect();
        }

        vTaskDelay((WIFI_TASK_PERIOD_MS / portTICK_PERIOD_MS));
    }
}

