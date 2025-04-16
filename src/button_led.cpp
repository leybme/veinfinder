#include "button_led.h"
#include "led_control.h"
#include "pins.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void buttonUp()
{
    if (systemEnabled)
    {
        increaseBrightness();
    }
}
void buttonDown()
{
    if (systemEnabled)
    {
        decreaseBrightness();
    }
}
void selectClick()
{
    Serial.println("Select button clicked");
    if (systemEnabled)
    {
        force_led_on(1000); // Force the LED on for 1 second
    }
    // Add your select button functionality here
}
void selectDoubleClick()
{
    Serial.println("Select button double clicked");
    if (systemEnabled)
    {
        nextPattern(); // Change to the next pattern
    }
}

void selectLongPress()
{
    Serial.println("Select button long pressed");
    systemEnabled = !systemEnabled; // Toggle system enabled state
    Serial.println(systemEnabled ? "System enabled" : "System disabled");
    if (systemEnabled)
    {
        force_led_on(3000); // Force the LED on for 1 second
    }
    else
    {
        force_led_on(0); // Turn off the LED
    }

    // Add your long press functionality here
}

void buttonSelectCheck()
{
    static unsigned long lastPressTime = 0;
    static unsigned long pressStartTime = 0;
    static bool selectButtonHeld = false;
    static bool longPressHandled = false;

    bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);

    if (buttonPressed)
    {
        if (!selectButtonHeld)
        {
            pressStartTime = millis();
            selectButtonHeld = true;
            longPressHandled = false;
        }
        else if (!longPressHandled && (millis() - pressStartTime >= 2000))
        {
            selectLongPress();
            longPressHandled = true;
        }
    }
    else if (selectButtonHeld)
    { // Button just released
        unsigned long pressDuration = millis() - pressStartTime;
        if (pressDuration < 2000 && !longPressHandled)
        {
            if (millis() - lastPressTime < 500)
            {
                selectDoubleClick();
            }
            else
            {
                selectClick();
            }
        }
        lastPressTime = millis();
        selectButtonHeld = false;
    }
}

void buttonTask(void *pvParameters)
{
    pinMode(UP_BUTTON, INPUT_PULLUP);   // Set up button pins as input with pull-up resistors
    pinMode(DOWN_BUTTON, INPUT_PULLUP); // Set up button pins as input with pull-up resistors
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Set up button pins as input with pull-up resistors

    while (true)
    {
        if (digitalRead(UP_BUTTON) == LOW)
        {
            buttonUp();
            vTaskDelay(pdMS_TO_TICKS(50)); // Debounce
        }

        if (digitalRead(DOWN_BUTTON) == LOW)
        {
            buttonDown();
            vTaskDelay(pdMS_TO_TICKS(50)); // Debounce
        }

        buttonSelectCheck();

        if (force_time > 0 && millis() > force_time)
        {
            ledcWriteChannel(LEDC_CHANNEL, 255 - currentBrightness); // Set brightness to original value
            force_time = 0;                                          // Reset force time
            Serial.println("Force time expired, LED to original brightness");
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Polling interval

        // Endless loop
    }
}

void setupButtonTask()
{
    xTaskCreate(
        buttonTask,    // Task function
        "Button Task", // Name of the task
        2048,          // Stack size in words
        NULL,          // Task input parameter
        1,             // Priority of the task
        NULL           // Task handle
    );
}
