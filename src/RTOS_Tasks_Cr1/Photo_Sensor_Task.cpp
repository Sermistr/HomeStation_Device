/**
 * @file    Photo_Sensor_Task.cpp
 * @brief   Monitors an analog photo-sensor (photodiode / LDR) and updates
 *          the shared ambient-light level.
 *
 * @details
 * This task reads the ADC value from the configured photo-sensor pin and
 * stores it in sensorData.lightLevel (raw 0..4095). It also reports any
 * obvious read anomalies via the system event queue so that the system
 * check task can flag them on the status LED.
 *
 * @author  Sergei Lazarev
 * @date    2026-04-23
 * @version 1.0
 * @note    Designed for ESP32 platform running FreeRTOS.
 */

#include "Photo_Sensor_Task.h"


void PhotoSensorTask(void* pvParameters)
{
    pinMode(Pins::PHOTO_SENSOR_PIN, INPUT);
    analogReadResolution(12); // 0..4095

    SystemEvent_t event;
    int consecutiveZero = 0;

    for (;;)
    {
        int raw = analogRead(Pins::PHOTO_SENSOR_PIN);

        if (raw < 0 || raw > (int)PHOTO_SENSOR_FULL_SCALE)
        {
            event = {
                .name = PHOTO_SENSOR,
                .status = SYSTEM_ERROR
            };
            xQueueSend(systemEventQueue, &event, QUEUE_TIMEOUT);
        }
        else
        {
            if (xSemaphoreTake(dataMutex, MUTEX_TIMEOUT) == pdTRUE)
            {
                sensorData.lightLevel = (uint16_t)raw;
                xSemaphoreGive(dataMutex);
            }

            // Stuck at 0 for a long time usually means the pin is not an
            // ADC pin (GPIO5 on ESP32-WROOM has no ADC). Flag it as WARN.
            if (raw == 0)
            {
                consecutiveZero++;
            }
            else
            {
                consecutiveZero = 0;
            }

            event = {
                .name = PHOTO_SENSOR,
                .status = (consecutiveZero > 5) ? SYSTEM_WARN : SYSTEM_OK
            };
            xQueueSend(systemEventQueue, &event, QUEUE_TIMEOUT);
        }

        vTaskDelay(PHOTO_SENSOR_TASK_PERIOD_MS / portTICK_PERIOD_MS);
    }
}
