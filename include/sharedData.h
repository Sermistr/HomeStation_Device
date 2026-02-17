#pragma once


#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>


#define MUTEX_TIMEOUT pdMS_TO_TICKS(1000)       // 1 second timeout for mutex operations
#define QUEUE_TIMEOUT pdMS_TO_TICKS(1000)  // 1 second timeout for queue read


typedef struct 
{
    float temperature;
    float humidity;
    bool wifiConnected;
}SharedData_t;


extern SharedData_t sensorData;
extern SemaphoreHandle_t dataMutex;


enum System_Name
{
    DHT_SENSOR,
    WIFI,
    LCD_DISPLAY,
};

enum System_Status
{
    SYSTEM_OK = 0,
    SYSTEM_WARN,
    SYSTEM_ERROR,
};


typedef struct 
{
    System_Name name;
    System_Status status;
}SystemEvent_t;


extern QueueHandle_t systemEventQueue;


typedef struct
{
    System_Status DHTStatus;
    System_Status WifiStatus;
    System_Status LcdStatus;
}SystemStatus_t;

extern SystemStatus_t systemStatus;