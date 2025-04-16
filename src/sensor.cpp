#include "sensor.h"
#include "pins.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "led_control.h"

bool sensorDetected = false; // Flag to indicate if a sensor is detected
long startTime = 0;          // Variable to store the start time of the sensor detection

bool sensorCheck()
{
    return (digitalRead(SENSOR1) == LOW && digitalRead(SENSOR2) == LOW);
}

void sensorTask(void *pvParameters)
{
    // Setup
    pinMode(SENSOR1, INPUT); // Set sensor pins as input with pull-up resistors
    pinMode(SENSOR2, INPUT);
    // Loop
    while (true)
    {
        if (systemEnabled) // Check if the system is enabled
        {
            if (sensorCheck()) // Check if both sensors are LOW
            {
                // safety check to prevent system from being enabled for more than 30 seconds
                if (startTime == 0) // Check if startTime is not set
                {
                    startTime = millis(); // Set the start time
                }
                else if (millis() - startTime >= 60000) 
                {
                    startTime = 0;         // Reset the start time
                    systemEnabled = false; // Disable the system
                    Serial.println("System disabled due to 60 second safety turn off.");
                }
                sensorDetected = true; // Set the flag if both sensors are LOW
                turnOnLeds();          // Call the function to turn on the LED
                // Serial.println("Sensor detected!"); // Print message to Serial Monitor
            }
            if (!sensorCheck()) // Check if either sensor is HIGH
            {
                startTime = 0;          // Reset the start time
                sensorDetected = false; // Reset the flag if either sensor is HIGH
                turnOffLeds();          // Call the function to turn off the LED
                // Serial.println("No sensor detected!"); // Print message to Serial Monitor
            }
        }
        else
        {
            sensorDetected = false;         // Reset the flag if the system is disabled
            turnOffLeds();                  // Call the function to turn off the LED
            vTaskDelay(pdMS_TO_TICKS(500)); 
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Delay for 1 second
    }
}

void setupSensorTask()
{

    // Initialize the sensors
    xTaskCreate(
        sensorTask,    // Task function
        "Sensor Task", // Name of the task
        2048,          // Stack size in words
        NULL,          // Task input parameter
        1,             // Priority of the task
        NULL           // Task handle
    );
}