/**
 * @file Telegram_Task.h
 * @brief FreeRTOS task interface for Telegram bot interactions.
 * 
 * @details
 * This header defines the interface for the Telegram task, which manages
 * communications with a Telegram bot. The task periodically checks for new
 * messages and responds with sensor data. It also includes a function to handle
 * incoming messages and execute corresponding actions.
 * 
 * @author  Sergei Lazarev
 * @date    2026-02-17
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
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h> 

/* Macros --------------------------------------------------------------------*/

/**
 * @brief Telegram task execution period in milliseconds.
 */
#define TELEGRAM_TASK_PERIOD_MS 1000 // Telegram task period in milliseconds


/* Public Function Prototypes ------------------------------------------------*/
/**
 * @brief FreeRTOS task responsible for Telegram bot interactions.
 *
 * @param pvParameters Pointer to task parameters (unused).
 */
void TelegramTask(void *pvParameters);

/**
 * @brief Handles new messages received by the Telegram bot.
 *
 * @param numNewMessages The number of new messages to process.
 */
void HandleNewMessages(int numNewMessages);