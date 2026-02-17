/**
 * @file    DHT_Sensor_Task.cpp
 * @brief   Monitors the DHT sensor and updates temperature and humidity readings.
 *
 * @details
 * This task reads data from the DHT sensor and updates the shared sensorData structure.
 * It also reports any errors via a FreeRTOS queue.
 *
 *
 * @author  Sergei Lazarev
 * @date    2026-02-17
 * @version 1.0
 *
 * @note    Designed for FreeRTOS-based embedded system.
 */



#include "DHT_Sensor_Task.h"


// DHT Sensor Task
void DHTSensorTask(void* pvParameters)
{
    // Initialize DHT sensor
    DHT dht(Pins::DHT_SENSOR_PIN, DHT11);
    dht.begin();

    float humidity = 0.0;
    float temperature = 0.0;

    SystemEvent_t errorEvent;

    for (;;)
    {
        // Read temperature and humidity
        humidity = dht.readHumidity();
        temperature = dht.readTemperature();

        // Check if any reads failed and exit early (to try again).
        if (isnan(humidity) || isnan(temperature)) {
            // Send error event to the queue
            errorEvent = {
                .name = DHT_SENSOR,
                .status = SYSTEM_ERROR
            };
            xQueueSend(systemEventQueue, &errorEvent, QUEUE_TIMEOUT);
        }

        else
        {
            // Update shared data with mutex protection
            if (xSemaphoreTake(dataMutex, MUTEX_TIMEOUT) == pdTRUE) {
                sensorData.temperature = temperature;
                sensorData.humidity = humidity;
                xSemaphoreGive(dataMutex);
            }
            
            errorEvent = {
                .name = DHT_SENSOR,
                .status = SYSTEM_OK
            };
            xQueueSend(systemEventQueue, &errorEvent, QUEUE_TIMEOUT);
        }

        // Delay before the next reading
        vTaskDelay(DHT_SENSOR_TASK_PERIOD_MS / portTICK_PERIOD_MS); // Delay for 2 seconds
    }
}