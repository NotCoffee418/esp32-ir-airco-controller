#include <LittleFS.h>
#include <WebServer.h>

#include "web_helpers.h"

static const uint16_t SERVE_FILE_BUFFER_SIZE = 2048;

// Serve a file from LittleFS
// This is a helper function to serve files from LittleFS
// Params:
// server: the web server instance
// path: the path to the file to serve
// contentType: the content type of the file (e.g. "text/css", "text/html", "application/javascript")
void serveFile(WebServer& server, const char* path, const char* contentType) {
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
    char buffer[SERVE_FILE_BUFFER_SIZE];  // Only X bytes in RAM at a time
	uint16_t bytesRead = 0;
    while(file.available()) {
		bytesRead = file.readBytes(buffer, SERVE_FILE_BUFFER_SIZE);
		server.sendContent(buffer, bytesRead);
    }
    file.close();
}

bool requirePost(WebServer& server) {
    if (server.method() != HTTP_POST) {
        server.send(405, "text/plain", "Method not allowed");
        return false;
    }
    return true;
}

bool requireGet(WebServer& server) {
    if (server.method() != HTTP_GET) {
        server.send(405, "text/plain", "Method not allowed");
        return false;
    }
    return true;
}
