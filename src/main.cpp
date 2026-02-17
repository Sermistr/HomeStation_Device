#include <Arduino.h>
#include <SermiLed.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sharedData.h"


#include "RTOS_Tasks_Cr1/DHT_Sensor_Task.h"
#include "RTOS_Tasks_Cr1/LCD_Display_Task.h"
#include "RTOS_Tasks_Cr1/System_Check_Task.h"

#include "RTOS_Tasks_Cr0/Wifi_Task.h"
#include "RTOS_Tasks_Cr0/Telegram_Task.h"


// initialize FreeRTOS objects
SemaphoreHandle_t dataMutex;
QueueHandle_t systemEventQueue;


SharedData_t sensorData = {
    .temperature = 0.0,
    .humidity = 0.0,
    .wifiConnected = false
};

void setup() {
  Serial.begin(115200);
  // Create a Mutex for shared data access
  dataMutex = xSemaphoreCreateMutex();
  if (dataMutex == NULL)
  {
    // Handle error creating mutex
    Serial.println("Failed to create data mutex");
  }


  systemEventQueue = xQueueCreate(8, sizeof(SystemEvent_t)); // Create a queue for system events
  if (systemEventQueue == NULL)
  {
    // Handle error creating queue
    Serial.println("Failed to create system event queue");
  }

  // Create DHT Sensor Task
  xTaskCreatePinnedToCore(DHTSensorTask, "DHT Sensor Task", 4096, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(LCDDisplayTask, "LCD Display Task", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(SystemCheckTask, "System Status Task", 4096, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(WiFiTask, "WiFi Task", 4096, NULL, 3, NULL, 0);
  xTaskCreatePinnedToCore(TelegramTask, "Telegram Task", 8192, NULL, 2, NULL, 0);
}


void loop() {
  // put your main code here, to run repeatedly:

}
