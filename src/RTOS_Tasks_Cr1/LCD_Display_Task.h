/**
 * @file    LCDDisplayTask.h
 * @brief   FreeRTOS task and interface for LCD display management.
 *
 * @details
 * This module provides the interface for controlling the I2C LCD display.
 * The LCD task periodically updates display data using shared system values.
 * It also provides utility functions for LCD health check and data printing.
 *
 * The execution period is defined by LCD_DISPLAY_TASK_PERIOD_MS.
 *
 * @author  Sergei Lazarev
 * @date    2026-02-17
 * @version 1.0
 *
 * @note    Designed for ESP32 platform running FreeRTOS.
 */

#pragma once

/* Includes ------------------------------------------------------------------*/

#include <LiquidCrystal_I2C.h>
#include "sharedData.h"

/* Macros --------------------------------------------------------------------*/

/**
 * @brief LCD display task execution period in milliseconds.
 */
#define LCD_DISPLAY_TASK_PERIOD_MS   2000U

/* Public Function Prototypes ------------------------------------------------*/

/**
 * @brief FreeRTOS task responsible for LCD updates.
 *
 * @param pvParameters Pointer to task parameters (unused).
 */
void LCDDisplayTask(void* pvParameters);

/**
 * @brief Performs LCD initialization and health check.
 *
 * @return true  LCD is working correctly.
 * @return false LCD initialization or communication failed.
 */
bool lcdCheck(void);

/**
 * @brief Prints temperature and humidity values on LCD.
 *
 * @param t Temperature value.
 * @param h Humidity value.
 */
void lcdPrint(float t, float h);

/**
 * @brief Reinitializes or refreshes the LCD display.
 */
void lcdReload(void);
