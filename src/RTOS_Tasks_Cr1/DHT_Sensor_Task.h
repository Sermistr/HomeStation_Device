#pragma once


#include <DHT.h>
#include "config.h"
#include "sharedData.h"


#define DHT_SENSOR_TASK_REC 2000    // Task execution period in milliseconds


void DHTSensorTask(void* pvParameters);