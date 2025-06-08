#pragma once

#include <Arduino.h>

struct DiagnosticData {
    String ramUsage;
    String flashUsage;
    String uptime;
    bool wifiConnected;
    int rssi;
    float deviceTemperature;
};

void printDiagnosticData();
DiagnosticData getDiagnosticData();