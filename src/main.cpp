#include <Arduino.h>
#include <SPI.h>
#include "led_control.h"
#include "sensor.h"
#include "button_led.h"
#include "pins.h"

void printInfo(); // Forward declaration of printInfo function
void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  Serial.begin(115200);
  ledInit(); // Initialize LED control
  setupSensorTask(); // Initialize sensor task
  setupButtonTask(); // Initialize button task
  printInfo(); // Print initial information
}

void loop() {

  delay(5000); // Main loop does nothing, all work is done in tasks
  if (systemEnabled) {
    printInfo();
  } else {
    Serial.println("System: Disabled");
  }
}

void printInfo()
{
  Serial.print("Status: Enabled | Pattern: ");
  Serial.print(currentPattern, BIN);
  Serial.print(" | Brightness: ");
  Serial.print(currentBrightness);
  Serial.print(" | Setting brn: ");
  Serial.print(settingBrightness);
  Serial.print(" | Sensor: ");
  Serial.println(sensorDetected ? "Detected" : "None");
}
