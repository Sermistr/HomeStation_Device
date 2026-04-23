#pragma once 


namespace Pins
{
    // Pin definitions
    constexpr int RED_LED_PIN       = 2; // Built-in LED pin
    constexpr int GREEN_LED_PIN     = 3; // Button pin
    constexpr int DHT_SENSOR_PIN    = 4; // DHT11 data pin
    constexpr int PHOTO_SENSOR_PIN  = 33; // Photodiode/LDR analog input
                                         // NOTE: GPIO5 is NOT an ADC pin on
                                         // ESP32-WROOM; move to GPIO34/36 if
                                         // you see constant zero readings.
}