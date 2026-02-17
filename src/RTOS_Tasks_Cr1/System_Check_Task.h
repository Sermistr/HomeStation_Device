/**
 * @file    SystemCheckTask.h
 * @brief   FreeRTOS task for overall system supervision.
 *
 * @details
 * This module provides the interface for monitoring the global system state.
 * The task evaluates subsystem statuses (WiFi, sensors, display, etc.),
 * prints diagnostic information to Serial, and controls LED indication.
 *
 * The LED behavior reflects the current system health state.
 *
 * @author  Sergei Lazarev
 * @date    2026-02-17
 * @version 1.0
 *
 * @note    Designed for ESP32 platform running FreeRTOS.
 */

#pragma once

/* Includes ------------------------------------------------------------------*/

#include "sharedData.h"
#include <SermiLed.h>

/* Public Function Prototypes ------------------------------------------------*/

/**
 * @brief FreeRTOS task responsible for monitoring system state.
 *
 * @param pvParameters Pointer to task parameters (unused).
 */
void SystemCheckTask(void* pvParameters);

/**
 * @brief Prints current system status to Serial output.
 */
void PrintSystemStatus(void);

/**
 * @brief Controls LED indication according to system state.
 *
 * @param led Reference to LED control object.
 */
void LedIndicateSystemStatus(Led& led);
