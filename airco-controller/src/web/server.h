#pragma once

class WebServer;

void webServerSetup(bool includeHotspotCapture);
void handleWebRequests();
WebServer* getServerInstance();