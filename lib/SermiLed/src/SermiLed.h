#pragma once 

#include <Arduino.h>


// Define a structure to represent an LED
// This structure holds the pin number and the current state of the LED (on/off)
struct Led
{
    uint8_t pin;
    bool state;
};

// Function prototypes
void LedInit(Led &led, uint8_t pin);                // Initializes the LED by setting the pin mode and initial state
void LedOn(Led &led);                               // Turns the LED on by setting the pin HIGH and updating the state            
void LedOff(Led &led);                              // Turns the LED off by setting the pin LOW and updating the state
void LedToggle(Led &led);                           // Toggles the LED state between on and off
void LedBlink(Led &led, uint16_t duration);         // Blinks the LED for the specified duration
bool IsLedOn(const Led &led);                       // Returns true if the LED is on, false otherwise
void SetBrightnessFromPort(Led &led, uint8_t brightness); // Sets the brightness of the LED using PWM control based on the specified brightness value (0-255)
