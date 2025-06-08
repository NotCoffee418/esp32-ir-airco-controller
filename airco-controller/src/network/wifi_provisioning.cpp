// Acts as parent to WebServer when we do not have a network connection

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESP32Ping.h>
#include "device_identity.h"
#include "storage/config.h"
#include "wifi_provisioning.h"

// Private functions
bool _hasHealthyConnection();

// Global private variables
DNSServer _dnsServer;


// Private Constants
// ms to wait for connection recovery
const unsigned long _PATIENCE_MS = 35000;
// ms to wait between connection recovery checks
const uint16_t _CONNECTION_RECOVERY_CHECK_INTERVAL_MS = 1000;
// Number of idle checks to perform (tied to _IDLE_CHECK_INTERVAL_MS)
const uint8_t _IDLE_CHECK_LIMIT = 50;
// Interval between idle checks (tied to _IDLE_CHECK_LIMIT)
const uint16_t _IDLE_CHECK_INTERVAL_MS = 100;
// Number of pings to perform
const uint8_t _PING_RETRIES = 5;
// ms to wait between ping attempts (short, within _PATIENCE_MS)
const uint16_t _PING_INTERVAL_MS = 500;


// Full check for wifi state and handle device restarts where needed.
// Should be run after startup WiFi actions and periodically.
WifiState determineWifiState() {
    Config config;
    getConfig(config);

    // If we don't have credentials, we can't connect
    if (config.ssid.length() == 0) {
        // Ensure we're in hotspot mode (excessive but low effort)
        if (!config.bootInHotspotMode) {
            setBootInHotspotMode(true);
            Serial.println("WiFi: Questionable state. Rebooting in hotspot mode.");
            ESP.restart();
            return WIFI_UNDEFINED;
        }
        return WIFI_HOTSPOT_MODE;
    }

    wl_status_t wifiStatus = WiFi.status();

    // Wait 5 seconds for idle status to be clear
    for (uint8_t i = 0; i < _IDLE_CHECK_LIMIT; i++) {
        // Hold while we don't have a clear status
        // These are brief statuses, we shouldn't run into them often
        if (wifiStatus != WL_IDLE_STATUS && wifiStatus != WL_CONNECTION_LOST) {
            break;
        }

        // Wait and refresh status
        delay(_IDLE_CHECK_INTERVAL_MS);
        wifiStatus = WiFi.status();

        // Shouldn't happen, but if it does, crash and reboot
        if (i == (_IDLE_CHECK_LIMIT - 1)) {
            Serial.println("WiFi: Failed to determine WiFi state. Stuck on idle status.");
            setBootInHotspotMode(true);
            delay(1000);
            ESP.restart();
            return WIFI_UNDEFINED;
        }    
    }

    // Handle connected
    if (_hasHealthyConnection()) {
        // If we are connected but still running hotspot, reboot device.
        // Causes: 
        // - Router was temporarily unreachable, so we started hotspot.
        // - We just set or changed credentials and managed to connect, so we switch to STA only mode.
        if (config.bootInHotspotMode) {            
            setBootInHotspotMode(false);
            Serial.println("WiFi: Connected. Rebooting to disable hotspot mode.");
            ESP.restart();
            return WIFI_UNDEFINED;
        }
        return WIFI_CONNECTED;
    }

    // We're not connected to a network
    // Disconnection may loop between:
    // WL_CONNECTION_LOST, WL_DISCONNECTED and WL_IDLE_STATUS
    // All are treated the same at this point.

    // We're in hotspot mode and not connected to a network.
    // We don't need to restart, but give user the opportunity to change credentials.
    if (config.bootInHotspotMode) {
        return WIFI_HOTSPOT_MODE;
    }

    // We're not in hotspot mode, but are disconnected.
    // This implies we were connected this session but lost connection.
    // Wait 30 seconds to try and reconnect, then reboot in hotspot mode.

    unsigned long dcStartTime = millis();
    Serial.println("WiFi: Disconnected. Waiting for connection recovery...");
    while (millis() - dcStartTime < _PATIENCE_MS) {
        // Wait and refresh status
        delay(_CONNECTION_RECOVERY_CHECK_INTERVAL_MS);

        // Connection recovered, no problems
        if (_hasHealthyConnection()) {
            Serial.println("WiFi: Reconnected.");
            return WIFI_CONNECTED;
        }

        Serial.println("WiFi: Still disconnected. Waiting...");
    }

    // If we're still disconnected, reboot in hotspot mode.
    setBootInHotspotMode(true);
    Serial.println("WiFi: Prolonged disconnection. Rebooting in hotspot mode.");
    ESP.restart();
    return WIFI_UNDEFINED; 
}

void startHotspot() {
    // Hardcoded IP address for hotspot config page.
    IPAddress apIp(192, 168, 255, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(apIp, apIp, subnet);

    // AP while also being able to connect to a network
    WiFi.mode(WIFI_AP_STA);

    // Set up access point with PIN as password
    String apName = getSetupHotspotName();
    bool apStarted = WiFi.softAP(apName.c_str());
    if (!apStarted) {
        Serial.println("AP: Failed to start access point");
        delay(1000);
        ESP.restart();
        return;
    }
    Serial.println("AP started: " + String(apName));
    Serial.println("AP IP: " + apIp.toString());

    // Set up the DNS server to capture all requests to the access point
    bool dnsStarted = _dnsServer.start(53, "*", apIp);
    if (!dnsStarted) {
        Serial.println("AP: Failed to start DNS server");
        delay(1000);
        ESP.restart();
        return;
    }
}

bool startWifiConnection() {
    return true;
}

// Should be called in loop() when in hotspot mode.
void handleHotspotDnsRequests() {
    _dnsServer.processNextRequest();
}


// Check if we're connected correctly to a network.
bool _hasHealthyConnection() {
    if (WiFi.status() != WL_CONNECTED) return false;
    if (WiFi.localIP() == IPAddress(0, 0, 0, 0)) return false;

    // Ping until we get a response with limited retries.
    for (uint8_t i = 0; i < _PING_RETRIES; i++) {
        if (Ping.ping(WiFi.gatewayIP())) {
            return true;
        }
        delay(_PING_INTERVAL_MS);
    }
    return false;
}