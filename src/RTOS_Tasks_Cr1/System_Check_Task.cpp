/**
 * @file    System_Check_Task.cpp
 * @brief   Monitors overall system state and handles error indication.
 *
 * @details
 * This task receives system events from WiFi, DHT sensor, and LCD tasks
 * via a FreeRTOS queue. It updates the shared SystemStatus_t structure
 * and controls a status LED. If any subsystem enters an error state,
 * the LED is activated and diagnostic information is printed to Serial.
 *
 * @author  Sergei Lazarev
 * @date    2026-02-17
 * @version 1.0
 *
 * @note    Designed for FreeRTOS-based embedded system.
 */


#include "System_Check_Task.h"


// System Status Task: Listens for system events and updates overall system status
SystemStatus_t systemStatus
{
    .DHTStatus = SYSTEM_OK,
    .WifiStatus = SYSTEM_OK,
    .LcdStatus = SYSTEM_OK
};


// System Event structure to hold the latest event received from tasks
SystemEvent_t event = {
    .name = DHT_SENSOR,
    .status = SYSTEM_OK
};

bool printSystemStatus = true; // Flag to control printing system status to Serial


void SystemCheckTask(void *pv) {
    SystemEvent_t evt;
    Led statusLed;
    LedInit(statusLed, 2); // Initialize status LED on pin 2

    for (;;) {
        if (xQueueReceive(systemEventQueue, &evt, QUEUE_TIMEOUT) == pdTRUE) {
        switch (evt.name) {
            case WIFI:
                if (systemStatus.WifiStatus != evt.status) { 
                    systemStatus.WifiStatus = evt.status;
                    printSystemStatus = true; // Set flag to print status on change
                }
                break;
            case DHT_SENSOR:
                if (systemStatus.DHTStatus != evt.status) { 
                    systemStatus.DHTStatus = evt.status;
                    printSystemStatus = true; // Set flag to print status on change
                }
                break;
            case LCD_DISPLAY:
                if (systemStatus.LcdStatus != evt.status) { 
                    systemStatus.LcdStatus = evt.status;
                    printSystemStatus = true; // Set flag to print status on change
                }
                break;
            default:
                break;
            }
            
            PrintSystemStatus();
            LedIndicateSystemStatus(statusLed); // Update LED based on system status
        }
    }
}

void PrintSystemStatus() {
    if (!printSystemStatus) 
    {
        return; // Only print if there's a change in status
    }
    Serial.print("WiFi: ");
    Serial.print(systemStatus.WifiStatus == SYSTEM_OK ? "OK" : (systemStatus.WifiStatus == SYSTEM_WARN ? "WARN" : "ERR"));
    Serial.print(" | DHT: ");
    Serial.print(systemStatus.DHTStatus == SYSTEM_OK ? "OK" : (systemStatus.DHTStatus == SYSTEM_WARN ? "WARN" : "ERR"));
    Serial.print(" | LCD: ");
    Serial.println(systemStatus.LcdStatus == SYSTEM_OK ? "OK" : (systemStatus.LcdStatus == SYSTEM_WARN ? "WARN" : "ERR"));
    printSystemStatus = false; // Reset flag after printing
}

void LedIndicateSystemStatus(Led &led) {
    if (systemStatus.WifiStatus == SYSTEM_ERROR || systemStatus.DHTStatus == SYSTEM_ERROR || systemStatus.LcdStatus == SYSTEM_ERROR) {
        LedOn(led); // Turn on LED if any system is in error state
    } else {
        LedOff(led); // Turn off LED if all systems are OK or in warning state
    }
}