#pragma once

#include "sharedData.h"
#include <SermiLed.h>


void SystemCheckTask(void* pvParameters);
void PrintSystemStatus();
void LedIndicateSystemStatus(Led &led);