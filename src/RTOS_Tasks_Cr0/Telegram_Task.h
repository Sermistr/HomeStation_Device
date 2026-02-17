#pragma once

#if __has_include("WiFiConfig.h")
    #include "WiFiConfig.h"
#else
    #include "WificonfigTemplate.h"
    #warning "WiFiConfig.h not found. Using template values."
#endif

#include "sharedData.h"
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h> 


#define TELEGRAM_TASK_REC 1000 // Telegram task period in milliseconds

void TelegramTask(void *pvParameters);
void HandleNewMessages(int numNewMessages);