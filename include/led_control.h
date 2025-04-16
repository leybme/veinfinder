#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>

#define LEDC_CHANNEL 0
#define PWM_FREQ     5000   // 5kHz frequency
#define PWM_RES      8      // 8-bit resolution (0-255)

extern bool systemEnabled; // Flag to indicate if the system is enabled
extern bool ledOn;        // Flag to indicate if the LED is on
extern int currentPattern;     // Current LED pattern index
extern int settingBrightness; // Setting brightness level
extern int currentBrightness; // Current brightness level
extern long force_time; // Time to force the LED on
void setBrightness(uint8_t brightness);
void setLedPattern(uint16_t pattern);
void ledInit();
void increaseBrightness();
void decreaseBrightness();
// extern int currentPattern; // Current LED pattern index
// extern int currentBrightness; // Current brightness level
void turnOnLeds();
void turnOffLeds();
void nextPattern();
void force_led_on(int duration_ms);


#endif // LED_CONTROL_H