#pragma once

#include <Arduino.h>

// Public enums
enum WifiState {
    WIFI_UNDEFINED = 0,
    WIFI_CONNECTED = 1,
    WIFI_HOTSPOT_MODE = 2,
};

WifiState determineWifiState();
void startHotspot();
bool startWifiConnection(bool setMode);
void handleHotspotDnsRequests();
