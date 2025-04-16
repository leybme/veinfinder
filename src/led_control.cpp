#include "led_control.h"
#include "sensor.h"
#include "pins.h"
#include <EEPROM.h>
#include <SPI.h>

int currentPattern = 0;
long force_time = 0;                                                                                       // Current LED pattern index
int currentBrightness = 10;                                                                                // Current brightness level
int settingBrightness = 100;                                                                               // Setting brightness level                                                                                          // LED state (0: off, 1: on)
bool systemEnabled = false;                                                                                // System enabled flag
uint16_t ledPattern[4] = {0b1110101010101011, 0b1111111111111111, 0b1010101010101010, 0b0101010101010101}; // LED patterns

// Send 16-bit data to STP16CP05
void sendData(uint16_t data)
{
    digitalWrite(LATCH_PIN, LOW);
    SPI.transfer16(data);
    digitalWrite(LATCH_PIN, HIGH);
    digitalWrite(LATCH_PIN, LOW);
    Serial.print("Data sent: ");
    Serial.println(data, BIN); // Print the sent data in binary format
}
void loadSettingBrightness()
{
    EEPROM.begin(512);                  // Initialize EEPROM with size 512 bytes
    settingBrightness = EEPROM.read(0); // Read the brightness value from address 0
    EEPROM.end();                       // Close the EEPROM
    Serial.print("Setting brightness loaded: ");
    Serial.println(settingBrightness);
}

void ledInit()
{
    loadSettingBrightness(); // Load brightness setting from EEPROM
    // Initialize SPI
    SPI.begin(6, -1, 7, LATCH_PIN); // SCK = GPIO6, MOSI = GPIO7

    // Initialize LED pins
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(OE_PIN, OUTPUT);
    // New LEDC setup using ledcAttach (merged ledcSetup and ledcAttachPin)
    ledcAttachChannel(OE_PIN, PWM_FREQ, PWM_RES, LEDC_CHANNEL); // Set up PWM on OE pin
    ledcWriteChannel(LEDC_CHANNEL, 0);                          // Set default brightness
    digitalWrite(LATCH_PIN, LOW);
    setLedPattern(0b1000000000000001);   // Turn on all red LEDs
    delay(1000);                         // Wait for 1 second
    setLedPattern(0x5555);               // Turn off all LEDs
    setBrightness(0); // Restore the previous brightness level
}
void setLedPattern(uint16_t pattern)
{
    sendData(pattern);
}
void setBrightness(uint8_t brightness)
{
    if (brightness == currentBrightness)
        return;                                       // Avoid unnecessary updates
    ledcWriteChannel(LEDC_CHANNEL, 255 - brightness); // Set default brightness
    currentBrightness = brightness;                   // Update current brightness
    Serial.print("Brightness set to: ");
    Serial.println(currentBrightness);
}
// save settingBrightness to EEPROM
void saveSettingBrightness()
{
    EEPROM.begin(512);                  // Initialize EEPROM with size 512 bytes
    EEPROM.write(0, settingBrightness); // Save the brightness value to address 0
    EEPROM.commit();                    // Commit the changes to EEPROM
    EEPROM.end();                       // Close the EEPROM
    Serial.print("Setting brightness saved: ");
    Serial.println(settingBrightness);
}

void increaseBrightness()
{
    // uint8_t currentBrightness = ledcRead(LEDC_CHANNEL);
    settingBrightness += 1; // Increase brightness by 1

    // if bright ness is 255, blink the led for 1 second
    if (settingBrightness >= 255)
    {
        settingBrightness = 255; // Set brightness to 255
        for (int i = 0; i < 5; i++)
        {
            ledcWriteChannel(LEDC_CHANNEL, 128); // Set brightness to 255
            delay(100);
            ledcWriteChannel(LEDC_CHANNEL, 255); // Set brightness to 0
            delay(100);
        }
    }
    force_led_on(1000);      // Force the LED on for 1 second
    saveSettingBrightness(); // Save the brightness value to EEPROM
}
void decreaseBrightness()
{
    // uint8_t currentBrightness = ledcRead(LEDC_CHANNEL);
    settingBrightness -= 1; // Decrease brightness by 5
    // if bright ness is 0, blink the led for 1 second
    if (settingBrightness <= 0)
    {
        settingBrightness = 0; // Set brightness to 0
        for (int i = 0; i < 5; i++)
        {
            ledcWriteChannel(LEDC_CHANNEL, 128); // Set brightness to 255
            delay(100);
            ledcWriteChannel(LEDC_CHANNEL, 255); // Set brightness to 0
            delay(100);
        }
    }
    force_led_on(1000);      // Force the LED on for 1 second
    saveSettingBrightness(); // Save the brightness value to EEPROM
}
void turnOffLeds()
{
    setBrightness(0); // Set brightness to 0
}
void turnOnLeds()
{
    setBrightness(settingBrightness); // Restore the previous brightness level
}

void nextPattern()
{
    currentPattern = (currentPattern + 1) % 4; // Cycle through patterns
    setLedPattern(ledPattern[currentPattern]); // Set the new pattern
    Serial.print("Pattern changed to: ");
    Serial.println(currentPattern); // Print the new pattern in binary format
    force_led_on(1000);             // Force the LED on for 1 second
}
void force_led_on(int duration_ms)
{

    if (settingBrightness == 0)
    {
        ledcWriteChannel(LEDC_CHANNEL, 255 - 10);
    }
    else
    {
        ledcWriteChannel(LEDC_CHANNEL, 255 - settingBrightness);
    }
    force_time = millis() + duration_ms; // Set the force time to the current time plus the duration
    // print force_time
    Serial.print("Force time set to: ");
    Serial.println(force_time);
}