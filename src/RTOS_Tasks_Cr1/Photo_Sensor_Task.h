/**
 * @file    Photo_Sensor_Task.h
 * @brief   FreeRTOS task for photodiode / LDR analog data acquisition.
 *
 * @details
 * This module provides the interface for the photo-sensor task.
 * The task periodically reads the ambient light level via ADC and
 * updates the shared sensorData.lightLevel field (raw 0..4095).
 *
 * The execution period is defined by PHOTO_SENSOR_TASK_PERIOD_MS.
 *
 * @author  Sergei Lazarev
 * @date    2026-04-23
 * @version 1.0
 * @note    Designed for ESP32 platform running FreeRTOS.
 */

#pragma once

/* Includes ------------------------------------------------------------------*/

#include <Arduino.h>
#include "config.h"
#include "sharedData.h"

/* Macros --------------------------------------------------------------------*/

/**
 * @brief Photo-sensor task execution period in milliseconds.
 */
#define PHOTO_SENSOR_TASK_PERIOD_MS  2000U

/**
 * @brief Full-scale value of the ESP32 ADC at 12-bit resolution.
 */
#define PHOTO_SENSOR_FULL_SCALE      4095U

/* Public Function Prototypes ------------------------------------------------*/

/**
 * @brief FreeRTOS task responsible for reading the photo-sensor.
 *
 * @param pvParameters Pointer to task parameters (unused).
 */
void PhotoSensorTask(void* pvParameters);
