#include <OneWire.h>
#include <DallasTemperature.h>
#include <float.h>

#define TEMPERATURE_SENSOR_PIN 13
OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature sensors(&oneWire);

// Global private variables
static float _tempC = FLT_MAX;

void setupTemperatureSensor() {
  sensors.begin();
}

// Reading takes 750ms and locks the ESP32 for that time.
// Use getCachedTempC() instead.
// This gets called in main.cpp loop()
float getFreshTempC() {
    sensors.requestTemperatures();
    _tempC = sensors.getTempCByIndex(0);
    return _tempC;
}

// Prefered use this
float getCachedTempC() {
    if (_tempC == FLT_MAX) {
        // No cached value yet, get fresh one
        return getFreshTempC();
    }
    return _tempC;
}