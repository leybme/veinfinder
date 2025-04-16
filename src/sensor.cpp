#include "sensor.h"
#include "pins.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "led_control.h"

bool sensorDetected = false; // Flag to indicate if a sensor is detected

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
        if (sensorCheck()) // Check if both sensors are LOW
        {
            sensorDetected = true; // Set the flag if both sensors are LOW
            turnOnLeds();          // Call the function to turn on the LED
            // Serial.println("Sensor detected!"); // Print message to Serial Monitor
        }
        if (!sensorCheck()) // Check if either sensor is HIGH
        {
            sensorDetected = false; // Reset the flag if either sensor is HIGH
            turnOffLeds();          // Call the function to turn off the LED
            // Serial.println("No sensor detected!"); // Print message to Serial Monitor
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