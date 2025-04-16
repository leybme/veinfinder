#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

#define SENSOR1 8
#define SENSOR2 14

bool sensorCheck();
void sensorTask(void *pvParameters);
void setupSensorTask();
extern bool sensorDetected; // Flag to indicate if a sensor is detected

#endif // SENSOR_H