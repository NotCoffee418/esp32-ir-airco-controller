#pragma once

#include <Arduino.h>

// Wifi credentials are stored in LittleFS
// hasCredentials: false if we have no credentials defined
// ssid: SSID of the network to connect to
// password: Password of the network to connect to
struct WifiCredentials {
    bool hasCredentials;
    String ssid;
    String password;
};

WifiCredentials getWifiCredentials();

void setBootInHotspotMode(bool bootInHotspotMode);
bool getBootInHotspotMode();