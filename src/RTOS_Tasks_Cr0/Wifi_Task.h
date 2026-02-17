/**
 * @file Wifi_Task.h
 * @brief Header for Wi-Fi management task in FreeRTOS.
 *
 * This file defines the interface for the Wi-Fi task, which is responsible for
 * managing the Wi-Fi connection, monitoring its status, and communicating events
 * to the system event queue. It includes necessary headers and defines constants
 * for task timing.
 *
 * @author Serghei
 * @date 2024-06-01
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

