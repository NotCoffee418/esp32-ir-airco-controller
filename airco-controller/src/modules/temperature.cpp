#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_SENSOR_PIN 13
OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature sensors(&oneWire);

void setupTemperatureSensor() {
  sensors.begin();
}

float getTempC() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}