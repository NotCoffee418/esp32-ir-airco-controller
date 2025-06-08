#include "page_configure_wifi.h"
#include "web_helpers.h"
#include "storage/config.h"
#include "network/wifi_provisioning.h"
#include <ArduinoJson.h>

// Private functions
String _getWifiConfigJSON();
void _saveWifiConfigJSON(String json);

void registerConfigureWifiPageHandlers(WebServer& server) {    
	server.on("/configure", [&server]() {
		serveFile(server, "/web/configure_wifi.html", "text/html");
	});

    server.on("/api/get-wifi-config", [&server]() {
        server.send(200, "application/json", _getWifiConfigJSON());
    });

    server.on("/api/set-wifi-config", [&server]() {
        // Take post body and parse as JSON
        String json = server.arg("plain");
        
        _saveWifiConfigJSON(json);

        // Start WiFi connection and restart device if valid
        if (startWifiConnection()) {
            server.send(200, "application/json", "{}");
            setBootInHotspotMode(false);
            ESP.restart();
        } else {
            server.send(500, "application/json", "{\"error\":\"Failed to start WiFi connection\"}");
        }
    });
}

String _getWifiConfigJSON() {
    Config config;
    getConfig(config);

    JsonDocument doc;
    doc["ssid"] = config.ssid;
    doc["password"] = config.password.length() > 0 ? "********" : "";
    doc["useDhcp"] = config.useDhcp;
    doc["networkDeviceIp"] = config.networkDeviceIp.toString();
    doc["networkGateway"] = config.networkGateway.toString();
    doc["networkSubnetMask"] = config.networkSubnetMask.toString();
    doc["networkDnsServer"] = config.networkDnsServer.toString();

    String json;
    serializeJson(doc, json);
    return json;
}

void _saveWifiConfigJSON(String json) {
    JsonDocument doc;
    deserializeJson(doc, json);

    // Get existing config
    Config config;
    getConfig(config);

    // Decide what password to use
    String inputPassword = doc["password"].as<String>();
    bool passwordChanged = false;
    if (inputPassword.length() == 0) {
        // Blank password is... valid.
        passwordChanged = true;
    } else {
        // if any character isn't *, then the password has changed
        for (int i = 0; i < inputPassword.length(); i++) {
            if (inputPassword[i] != '*') {
                passwordChanged = true;
                break;
            }
        }
    }

    // Update config
    config.ssid = doc["ssid"].as<String>();
    config.password = passwordChanged ? inputPassword : config.password;
    config.useDhcp = doc["useDhcp"].as<bool>();
    config.networkDeviceIp.fromString(doc["networkDeviceIp"].as<String>());
    config.networkGateway.fromString(doc["networkGateway"].as<String>());
    config.networkSubnetMask.fromString(doc["networkSubnetMask"].as<String>());
    config.networkDnsServer.fromString(doc["networkDnsServer"].as<String>());

    // Save config
    saveConfig(config);    
}