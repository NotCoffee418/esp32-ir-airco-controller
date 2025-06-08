#pragma once

#include <Arduino.h>

// Wifi credentials are stored in LittleFS
// hasCredentials: false if we have no credentials defined
// ssid: SSID of the network to connect to
// password: Password of the network to connect to
struct Config {
    bool bootInHotspotMode;
    String ssid;
    String password;
    bool useDhcp;
    IPAddress networkDeviceIp;
    IPAddress networkGateway;
    IPAddress networkSubnetMask;
    IPAddress networkDnsServer;
};

void saveConfig(const Config& config);
void getConfig(Config& config);

void setBootInHotspotMode(bool bootInHotspotMode);