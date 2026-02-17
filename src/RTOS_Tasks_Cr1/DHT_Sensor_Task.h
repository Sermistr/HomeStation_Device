/**
 * @file    DHTSensorTask.h
 * @brief   FreeRTOS task for DHT sensor data acquisition.
 *
 * @details
 * This module provides the interface for the DHT sensor task.
 * The task periodically reads temperature and humidity data
 * from the DHT sensor and updates shared system data structures.
 *
 * The execution period is defined by DHT_SENSOR_TASK_PERIOD_MS.
 *
 * @author  Sergei Lazarev
 * @date    2026-02-17
 * @version 1.0
 *
 * @note    Designed for ESP32 platform running FreeRTOS.
 */

#pragma once

/* Includes ------------------------------------------------------------------*/

#include <DHT.h>
#include "config.h"
#include "sharedData.h"

/* Macros --------------------------------------------------------------------*/

/**
 * @brief DHT sensor task execution period in milliseconds.
 */
#define DHT_SENSOR_TASK_PERIOD_MS   2000U

/* Public Function Prototypes ------------------------------------------------*/

/**
 * @brief FreeRTOS task responsible for reading DHT sensor data.
 *
 * @param pvParameters Pointer to task parameters (unused).
 */
void DHTSensorTask(void* pvParameters);
