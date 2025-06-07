#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>

#include "web/server.h"
#include "temp_wifi_setup.h" // temp
#include "network/setup_identity.h"

void setup() {
	// Initialize serial communication
	Serial.begin(115200);

	// Try to initialize filesystem, but don't worry if it fails
    if (!LittleFS.begin(true)) {
        throw std::runtime_error("Error mounting LittleFS");
    }


	// Setup WiFi
	tempWifiSetup();

	// Setup web server (Requires an IP address)
	webServerSetup();

	// Report device identity
	Serial.println("Device identity:");
	Serial.println("  MAC Address: " + String(WiFi.macAddress()));
	Serial.println("  Hotspot name: " + getSetupHotspotName());
	Serial.println("  Hotspot password: " + getSetupHotspotPassword());

}

void loop() {
	handleWebRequests();
}

// put function definitions here:
int myFunction(int x, int y) {
	return x + y;
}