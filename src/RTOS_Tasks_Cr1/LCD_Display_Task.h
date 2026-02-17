#pragma once 

#include <LiquidCrystal_I2C.h>
#include "sharedData.h"


#define LCD_DISPLAY_TASK_REC 2000    // Task execution period in milliseconds


void LCDDisplayTask(void* pvParameters);
bool lcdCheck();    // Checks if lcd is working correctly
void lcdPrint(float t, float h);
void lcdReload();