#pragma once

#include <Arduino.h>

struct DiagnosticData {
    String ramUsage;
    String flashUsage;
    String uptime;
    bool wifiConnected;
    int rssi;
    float deviceTemperature;
    String ssid;
    String deviceIp;
};

void printDiagnosticData();
DiagnosticData getDiagnosticData();