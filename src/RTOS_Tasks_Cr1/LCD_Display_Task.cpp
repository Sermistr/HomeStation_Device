#include "LCD_Display_Task.h"


LiquidCrystal_I2C lcd(0x27, 16, 2);


void LCDDisplayTask(void* pvParameters)
{
    Wire.begin();

    lcd.init();
    lcd.backlight();

    float temperature = 0.0;
    float humidity = 0.0;

    bool lcdWasCrushed = false; 

    SystemEvent_t event;

    for(;;)
    {
        if(lcdCheck())
        {
            if (xSemaphoreTake(dataMutex, MUTEX_TIMEOUT) == pdTRUE) {
                temperature = sensorData.temperature;
                humidity = sensorData.humidity;
                xSemaphoreGive(dataMutex);
            }
            
            event = {
                .name = LCD_DISPLAY,
                .status = SYSTEM_OK
            };

            xQueueSend(systemEventQueue, &event, QUEUE_TIMEOUT);

            if (lcdWasCrushed)
            {
                lcdReload();
                lcdWasCrushed = false;
            }
        }

        else
        {
            event = {
                .name = LCD_DISPLAY,
                .status = SYSTEM_ERROR
            };
            xQueueSend(systemEventQueue, &event, QUEUE_TIMEOUT);
            lcdWasCrushed = true;
        }
        
        lcdPrint(temperature, humidity);

        vTaskDelay(LCD_DISPLAY_TASK_REC / portTICK_PERIOD_MS); // Delay for 2 seconds
    }
}


bool lcdCheck() {
    Wire.beginTransmission(0x27);
    return (Wire.endTransmission() == 0);
}


void lcdPrint(float t, float h)
{
    static float lastTemp = 0.0;
    static float lastHum = 0.0;

    
    if (lastTemp != t)
    {
        lcd.setCursor(0, 0);
        lcd.printf("Temp: %.1f C", t);
        lastTemp = t;
    }
    
    if (lastHum != h)
    {
        lcd.setCursor(0, 1);
        lcd.printf("Hum: %.1f %%", h);
        lastHum = h;
    }
}


void lcdReload()
{
    lcd.init();
    lcd.backlight();
}