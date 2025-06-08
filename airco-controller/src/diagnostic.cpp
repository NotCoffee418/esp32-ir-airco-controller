#include <Arduino.h>
#include <WiFi.h>
#include "diagnostic.h"

void printDiagnosticData() {
    DiagnosticData data = getDiagnosticData();
    Serial.println("--------------------------------");
    Serial.println("RAM: " + data.ramUsage);
    Serial.println("Flash: " + data.flashUsage);
    Serial.println("Uptime: " + data.uptime);
    Serial.println("WiFi: " + data.wifiConnected ? "Connected" : "Hotspot Mode");
    if (data.wifiConnected) {
        Serial.println("RSSI: " + String(data.rssi) + " dBm");
        Serial.println("SSID: " + data.ssid);
    }
    Serial.println("Device IP: " + data.deviceIp);
    Serial.println("Device Temperature: " + String(data.deviceTemperature, 1) + "°C");
    Serial.println("--------------------------------");
}


DiagnosticData getDiagnosticData() {
    DiagnosticData data;
    
    // RAM - show used instead of free
    uint32_t totalRam = ESP.getHeapSize() / 1024;
    uint32_t freeRam = ESP.getFreeHeap() / 1024;
    uint32_t usedRam = totalRam - freeRam;
    data.ramUsage = String(usedRam) + " KB / " + String(totalRam) + " KB";
    
    // Flash - show used instead of free
    uint32_t totalFlash = ESP.getFlashChipSize() / 1024;
    uint32_t usedFlash = ESP.getSketchSize() / 1024;
    data.flashUsage = String(usedFlash) + " KB / " + String(totalFlash) + " KB";
    
    data.uptime = String(millis() / 60000) + " min";
    data.wifiConnected = WiFi.status() == WL_CONNECTED;
    data.rssi = data.wifiConnected ? WiFi.RSSI() : 0;
    data.deviceTemperature = temperatureRead();
    data.ssid = data.wifiConnected ? WiFi.SSID() : "";
    data.deviceIp = data.wifiConnected ? WiFi.localIP().toString() : WiFi.softAPIP().toString();
    return data;
}