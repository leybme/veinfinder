#include <Arduino.h>
#include <SPI.h>
#include "led_control.h"
#include "sensor.h"
#include "button_led.h"
#include "pins.h"


void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  Serial.begin(115200);
  ledInit(); // Initialize LED control
  setupSensorTask(); // Initialize sensor task
  setupButtonTask(); // Initialize button task
}

void loop() {

  delay(1000); // Main loop does nothing, all work is done in tasks
  Serial.println("Main loop running..."); // Print message to Serial Monitor
  if (systemEnabled) {
    // Check if the system is enabled
    Serial.println("System is enabled.");
  } else {
    // System is disabled
    Serial.println("System is disabled.");
  }
  Serial.println("Current Pattern: ");
  Serial.println(currentPattern, BIN); // Print current pattern in binary format
  Serial.println("Current Brightness: ");
  Serial.println(currentBrightness); // Print current brightness level
  Serial.println("Sensor Detected: ");
  Serial.println(sensorDetected ? "Yes" : "No"); // Print sensor detection status
}



