#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>

#include "web/server.h"
#include "temp_wifi_setup.h" // temp
#include "network/device_identity.h"
#include "network/wifi_provisioning.h"
#include "storage/config.h"
#include "diagnostic.h"
#include "storage/config.h"

// private constants
const unsigned long _WIFI_HEALTH_CHECK_INTERVAL_MS = 60000;

// Global private variables
static bool _inHotspotMode = false;
static unsigned long _lastWifiHealthCheck = 0;

void setup() {
	// Initialize serial communication
	Serial.begin(115200);

	// Try to initialize filesystem, but don't worry if it fails
    if (!LittleFS.begin(true)) {
		Serial.println("Error mounting LittleFS");
		delay(1000);
		ESP.restart();
		return;
    }

	// Load config
	Config config;
	getConfig(config);

	// Setup WiFi
	_inHotspotMode = config.bootInHotspotMode;
	if (_inHotspotMode) {
		startHotspot();
	} else {
		Serial.println("WiFi: Not in hotspot mode. Not implemented.");
		delay(1000);
		ESP.restart();
		return;
	}

	// Setup web server (Requires an IP address)
	webServerSetup(_inHotspotMode);

	// Report device identity
	Serial.println("MAC Address: " + String(WiFi.macAddress()));
	Serial.println("Hotspot name: " + getSetupHotspotName());
	Serial.println("Admin PIN: " + getSetupHotspotPassword());
	printDiagnosticData();
}

void loop() {
	if (_inHotspotMode) {
	// Handle web and hotspot DNS requests
		handleHotspotDnsRequests();
	}

	// Periodic WiFi health check in all modes
	// Will restart in correct mode if needed
	if (millis() - _lastWifiHealthCheck > _WIFI_HEALTH_CHECK_INTERVAL_MS) {
		
		Serial.println("Running WiFi health check...");
		WifiState wifiState = determineWifiState();
		switch (wifiState) {
			case WIFI_CONNECTED:
				Serial.println("WiFi: ok");
				break;
			case WIFI_HOTSPOT_MODE:
				Serial.println("WiFi: hotspot mode");
				break;
			default:
				Serial.println("WiFi: unknown state");
				break;
		}

		// Print system stats
		printDiagnosticData();

		// Reset timer
		_lastWifiHealthCheck = millis();
	}

	// Handle web requests
	handleWebRequests();

	
}