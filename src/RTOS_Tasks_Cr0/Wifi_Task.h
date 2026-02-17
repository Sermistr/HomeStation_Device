/**
 * @file Wifi_Task.h
 * @brief FreeRTOS task interface for Wi-Fi management.
 * 
 * @details
 * This header defines the interface for the Wi-Fi task, which manages
 * the Wi-Fi connection. The task periodically checks the connection status
 * and attempts to reconnect if necessary. It also includes a function to
 * handle Wi-Fi events and update shared data.
 * 
 * @author  Sergei Lazarev
 * @date    2026-02-17
 * @version 1.0
 * @note    Designed for ESP32 platform running FreeRTOS.
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "sharedData.h"

#if __has_include("WiFiConfig.h")
    #include "WiFiConfig.h"
#else
    #include "WificonfigTemplate.h"
    #warning "WiFiConfig.h not found. Using template values."
#endif

#include <WiFi.h>

/* Macros --------------------------------------------------------------------*/

/**
 * @brief Wi-Fi task execution period in milliseconds.
 */
#define WIFI_TASK_PERIOD_MS   5000


/* Public Function Prototypes ------------------------------------------------*/
/**
 * @brief FreeRTOS task responsible for managing Wi-Fi connection.
 *
 * This task attempts to connect to the specified Wi-Fi network and monitors
 * the connection status. It updates shared data and sends system events based
 * on the connection state.
 *
 * @param pv Pointer to task parameters (unused).
 */
void WiFiTask(void *pv);

