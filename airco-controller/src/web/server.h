#pragma once

class WebServer;

void webServerSetup();
void handleWebRequests();
void serveFile(const char* path, const char* contentType);