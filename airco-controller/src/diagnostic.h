#pragma once

#include <Arduino.h>

struct DiagnosticData {
    String ramUsage;
    String flashUsage;
    String uptime;
    bool wifiConnected;
    int rssi;
    float deviceTemperature;
    float sensorTemperature;
    String ssid;
    String deviceIp;
    String deviceIdentifier;
};

void printDiagnosticData();
DiagnosticData getDiagnosticData();