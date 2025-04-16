#include "button_led.h"
#include "led_control.h"
#include "pins.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


void buttonUp()
{
    increaseBrightness();

}
void buttonDown()
{
    decreaseBrightness();
}

void buttonTask(void *pvParameters)
{
    pinMode(UP_BUTTON, INPUT_PULLUP);   // Set up button pins as input with pull-up resistors
    pinMode(DOWN_BUTTON, INPUT_PULLUP); // Set up button pins as input with pull-up resistors
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Set up button pins as input with pull-up resistors
    unsigned long lastPressTime = 0;
    unsigned long pressStartTime = 0;
    bool selectButtonHeld = false;

    while (true)
    {
        if (digitalRead(UP_BUTTON) == LOW)
        {
            buttonUp();
            vTaskDelay(pdMS_TO_TICKS(100)); // Debounce
        }

        if (digitalRead(DOWN_BUTTON) == LOW)
        {
            buttonDown();
            vTaskDelay(pdMS_TO_TICKS(100)); // Debounce
        }

        if (digitalRead(BUTTON_PIN) == LOW)
        {
            if (!selectButtonHeld)
            {
                pressStartTime = millis();
                selectButtonHeld = true;
            }
            else if (millis() - pressStartTime >= 2000)
            { // Long press
                systemEnabled = !systemEnabled;
                Serial.println(systemEnabled ? "System Enabled" : "System Disabled");
                vTaskDelay(pdMS_TO_TICKS(500)); // Prevent immediate re-trigger
            }
        }
        else
        {
            if (selectButtonHeld)
            {
                unsigned long pressDuration = millis() - pressStartTime;
                if (pressDuration < 2000 && millis() - lastPressTime < 500)
                { // Double click
                  // currentPattern = (currentPattern + 1) % 3;
                  // Serial.print("Pattern changed to: ");
                  // Serial.println(currentPattern);
                }
                lastPressTime = millis();
            }
            selectButtonHeld = false;
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // Polling interval
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