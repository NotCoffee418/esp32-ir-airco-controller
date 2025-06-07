#include "server.h"

#include <Arduino.h>
#include <LittleFS.h>
#include <WebServer.h>

// Web server instance
WebServer server(80);

void webServerSetup() {
	// TODO: Make conditional homepage, maybe redirect?
	server.on("/configure_wifi", []() {
		serveFile("/web/configure_wifi.html", "text/html");
	});

	// Resources
	server.on("/css/water.min.css", []() {
		serveFile("/web/css/water.min.css", "text/css");
	});


	// Handle 404
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

// Serve a file from LittleFS
// This is a helper function to serve files from LittleFS
// Params:
// server: the web server instance
// path: the path to the file to serve
// contentType: the content type of the file (e.g. "text/css", "text/html", "application/javascript")
void serveFile(const char* path, const char* contentType) {
	File file = LittleFS.open(path, "r");

	// Handle 404
	if (!file) {
        server.send(404, "text/plain", "File not found");
		file.close();
        return;
    }

	// Set headers
	server.setContentLength(file.size());
	server.send(200, contentType, "");
  
	// Send file content
    char buffer[1024];  // Only 1KB in RAM at a time
	uint16_t bytesRead = 0;
    while(file.available()) {
		bytesRead = file.readBytes(buffer, 1024);
		server.sendContent(buffer, bytesRead);
    }
    file.close();
}