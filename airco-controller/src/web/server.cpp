#include <Arduino.h>
#include <LittleFS.h>
#include <WebServer.h>

#include "server.h"
#include "page_status.h"
#include "web_helpers.h"
#include "page_configure_wifi.h"
#include "page_login.h"
#include "api_ircontrol.h"
#include "api_apikey_management.h"

// Private functions
void _handleCaptivePortalDetection();
void _setupHotspotCaptureEndpoints();

// Web server instance
WebServer server(80);

void webServerSetup(bool includeHotspotCapture) {
	// Tell the server to collect Cookie headers
    const char* headerKeys[] = {"Cookie"};
    size_t headerKeysSize = sizeof(headerKeys) / sizeof(char*);
    server.collectHeaders(headerKeys, headerKeysSize);


	// Redirect index based on mode
	String indexRedirect = "/status";
	if (includeHotspotCapture) {
		_setupHotspotCaptureEndpoints();
		indexRedirect = "/configure";
	}

	server.on("/", [indexRedirect]() {
		server.sendHeader("Location", indexRedirect);
		server.send(302, "text/plain", "");
	});

	// Resources
	server.on("/css/water.min.css", []() {
		serveFile(server, "/web/css/water.min.css", "text/css");
	});

	// Register handler groups
	registerStatusPageHandlers(server);
	registerConfigureWifiPageHandlers(server);
	registerLoginPageHandlers(server);
	registerIrControlApiHandlers(server);
	registerApiKeyManagementApiHandlers(server);

	server.onNotFound([]() { 
		String uri = server.uri();
		String method = (server.method() == HTTP_GET) ? "GET" : "POST";
		Serial.println("404: " + method + " " + uri);
		server.send(404, "text/plain", "404: Not found");
	});

	server.begin();
}

void handleWebRequests() {
	server.handleClient();
}


// This is theoretically, not so much luck in practice.
// We'll leave it and put an IP address on the hardware.
void _setupHotspotCaptureEndpoints() {
	// Android captive portal detection
    server.on("/generate_204", _handleCaptivePortalDetection);
    server.on("/gen_204", _handleCaptivePortalDetection);
    
    // iOS/macOS captive portal detection
    server.on("/hotspot-detect.html", _handleCaptivePortalDetection);
    server.on("/library/test/success.html", _handleCaptivePortalDetection);
    
    // Windows captive portal detection
    server.on("/connecttest.txt", _handleCaptivePortalDetection);
    server.on("/redirect", _handleCaptivePortalDetection);
    
    // Firefox captive portal detection
    server.on("/canonical.html", _handleCaptivePortalDetection);
    server.on("/success.txt", _handleCaptivePortalDetection);
    server.on("/static/hotspot.txt", _handleCaptivePortalDetection);
}


void _handleCaptivePortalDetection() {
	String redirectUrl = "http://" + WiFi.softAPIP().toString() + "/configure";    
    server.sendHeader("Location", redirectUrl, true);
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "0");
    server.send(302, "text/plain", "Redirecting to setup page");
}