#pragma once


#include "sharedData.h"

#if __has_include("WiFiConfig.h")
    #include "WiFiConfig.h"
#else
    #include "WificonfigTemplate.h"
    #warning "WiFiConfig.h not found. Using template values."
#endif

#include <WiFi.h>


#define WIFI_TASK_REC 5000 // Wi-Fi task period in milliseconds

void WiFiTask(void *pv);

