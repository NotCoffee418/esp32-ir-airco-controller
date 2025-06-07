#ifndef SERVER_H
#define SERVER_H

class WebServer;

void webServerSetup();
void handleWebRequests();
void serveFile(const char* path, const char* contentType);

#endif