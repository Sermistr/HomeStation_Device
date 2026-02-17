#include <SermiLed.h>


void LedInit(Led &led, uint8_t pin)
{
    led.pin = pin;
    led.state = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}


void LedOn(Led &led)
{
    digitalWrite(led.pin, HIGH);
    led.state = true;
}


void LedOff(Led &led)
{
    digitalWrite(led.pin, LOW);
    led.state = false;
}


void LedToggle(Led &led)
{
    if (led.state)
    {
        LedOff(led);
    }
    else
    {
        LedOn(led);
    }
}

// Simple blink function 
// if FreeRTOS is not used, it will use delay() which is blocking
// If FreeRTOS is used, it will use vTaskDelay which is non-blocking
void LedBlink(Led &led, uint16_t duration)
{
    #if __has_include(<freertos/FreeRTOS.h>)
        LedOn(led);
        vTaskDelay(duration / portTICK_PERIOD_MS);
        LedOff(led);
    #else
        LedOn(led);
        delay(duration);
        LedOff(led);
    #endif
}


bool IsLedOn(const Led &led)
{
    return led.state;
}


// This function sets pwm brightness for the LED using the specified brightness value (0-255)
// If the project is based on Arduino, it will use analogWrite
// If the project is based on esp32 and FreeRTOS, it will use ledcWrite for PWM control 
void SetBrightnessFromPort(Led &led, uint8_t brightness)
{
    // if the project is based on Arduino, use analogWrite
    #if __has_include(<Arduino.h>) && !defined(ARDUINO_ARCH_ESP32)
        analogWrite(led.pin, brightness);
    #endif

    // if the project is based on esp32 use ledcWrite for PWM control
    #if defined(ARDUINO_ARCH_ESP32) && __has_include(<freertos/FreeRTOS.h>)
        // Configure the LEDC channel for PWM control
        const int ledChannel = 0; // Use channel 0 for the LED
        const int ledFrequency = 5000; // Set the PWM frequency to 5 kHz
        const int ledResolution = 8; // Set the PWM resolution to 8 bits    
        ledcSetup(ledChannel, ledFrequency, ledResolution);
        ledcAttachPin(led.pin, ledChannel); // Attach the LED pin to the LEDC channel
        ledcWrite(ledChannel, brightness); // Set the brightness (0-255)
    #endif

}