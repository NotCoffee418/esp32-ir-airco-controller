#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "config.h"

// Private functions
void _setFreshConfig();
template<typename T>
T _parseVal(const JsonDocument& doc, const char* key);

void saveConfig(const Config& config) {
    JsonDocument doc;
    
    doc["bootInHotspotMode"] = config.bootInHotspotMode;
    doc["ssid"] = config.ssid;
    doc["password"] = config.password;
    doc["useDhcp"] = config.useDhcp;
    doc["networkDeviceIp"] = config.networkDeviceIp.toString();
    doc["networkGateway"] = config.networkGateway.toString();
    doc["networkSubnetMask"] = config.networkSubnetMask.toString();
    doc["networkDnsServer"] = config.networkDnsServer.toString();
    
    String json;
    serializeJson(doc, json);
    
    // Save to LittleFS
    File file = LittleFS.open("/config.json", "w");
    if (!file) {
        Serial.println("config: Failed to open config file for writing");
        delay(1000);
        ESP.restart();
        return;
    }
    file.print(json);
    file.close();
}

void getConfig(Config& config) {
    // Load from LittleFS
    File file = LittleFS.open("/config.json", "r");
    if (!file) {
        Serial.println("config: Failed to load config file");
        _setFreshConfig();
        return;
    }

    // Read file content
    String json = file.readString();
    file.close();
    
    // Parse JSON
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);    
    if (error) {
        Serial.println("config: Failed to parse config JSON");
        _setFreshConfig();
        return;
    }
    
    // Set config
    config.bootInHotspotMode = _parseVal<bool>(doc, "bootInHotspotMode");
    config.ssid = _parseVal<String>(doc, "ssid");
    config.password = _parseVal<String>(doc, "password");
    config.useDhcp = _parseVal<bool>(doc, "useDhcp");
    
    // Convert string back to IPAddress
    config.networkDeviceIp.fromString(_parseVal<String>(doc, "networkDeviceIp"));
    config.networkGateway.fromString(_parseVal<String>(doc, "networkGateway"));
    config.networkSubnetMask.fromString(_parseVal<String>(doc, "networkSubnetMask"));
    config.networkDnsServer.fromString(_parseVal<String>(doc, "networkDnsServer"));
}

void setBootInHotspotMode(bool bootInHotspotMode) {
    Config config;
    getConfig(config);
    config.bootInHotspotMode = bootInHotspotMode;
    saveConfig(config);
}

// Save fresh config - fresh device or config corruption
void _setFreshConfig() {
    Serial.println("config: Saving fresh config! Rebooting...");
    
    // Save config with default values
    Config config = Config();
    config.bootInHotspotMode = true;
    config.useDhcp = true;
    config.networkSubnetMask = IPAddress(255, 255, 255, 0);
    config.networkDnsServer = IPAddress(1, 1, 1, 1);

    saveConfig(config);

    // Reboot
    ESP.restart();
}

// Parse value or assume corruption and refresh config
template<typename T>
T _parseVal(const JsonDocument& doc, const char* key) {
    if (doc[key].is<T>()) {
        return doc[key].as<T>();
    }
    
    // Failed, assume corruption and refresh config
    Serial.println("config: Failed to parse value for key: " + String(key));
    _setFreshConfig();
    return T{}; // Default constructor
}