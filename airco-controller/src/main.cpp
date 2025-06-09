#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>

#include "web/server.h"
#include "network/device_identity.h"
#include "network/wifi_provisioning.h"
#include "storage/config.h"
#include "diagnostic.h"
#include "storage/config.h"
#include "modules/temperature.h"
#include "modules/ir_remote.h"

// private constants
const unsigned long _WIFI_HEALTH_CHECK_INTERVAL_MS = 30000;

// Global private variables
static bool _inHotspotMode = false;
static unsigned long _lastWifiHealthCheck = 0;

void setup() {
	// Initialize serial communication
	Serial.begin(115200);
	Serial.println("Booting Airco Control Unit...");

	// Wait for everything to wake up 
	// wifi sometimes won't connect without this
	delay(3000);

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

	// Set WiFi to persistent mode to off (applies to all modes).
	// Prevents using old credentials despite explicitly defining them otherwise.
	// Persists between flashes and reboots in magic storage we do not want..
	// Manual wipe: esptool.py --chip esp32 erase_region 0x9000 0x6000
	WiFi.persistent(false);

	// Setup WiFi
	_inHotspotMode = config.bootInHotspotMode;
	if (_inHotspotMode) {
		startHotspot();
	} else {
		Serial.println("Requesting connection to SSID: " + String(config.ssid));
		bool connected = startWifiConnection(true);
		if (!connected) {
			Serial.println("WiFi: Failed to connect. Rebooting in hotspot mode.");
			setBootInHotspotMode(true);
			delay(1000);
			ESP.restart();
			return;
		}
	}

	// Setup web server (Requires an IP address)
	webServerSetup(_inHotspotMode);

	// setup modules
	setupTemperatureSensor();
	setupIrRemote();

	// Report device identity
	Serial.println("MAC Address: " + String(WiFi.macAddress()));
	Serial.println("Hotspot name: " + getSetupHotspotName());
	Serial.println("Admin PIN: " + getAccessPin());
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
		Serial.println("Health check at " + String(millis()));
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

		// Get fresh temperature reading and report it
		// Not just for health check, we also use this to update cached value.
		float tempC = getFreshTempC();
		Serial.println("Temperature Sensor: " + String(tempC));

		// Reset timer
		_lastWifiHealthCheck = millis();
	}

	// Handle web requests
	handleWebRequests();
}