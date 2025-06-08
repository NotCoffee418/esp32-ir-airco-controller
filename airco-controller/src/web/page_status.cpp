#include <ArduinoJson.h>
#include "diagnostic.h"
#include "page_status.h"
#include "web_helpers.h"

// Private functions
String _getDiagnosticDataJSON();

void registerStatusPageHandlers(WebServer& server) {
    server.on("/status", [&server]() {
        serveFile(server, "/web/status.html", "text/html");
    });
    server.on("/api/diagnostic-data", [&server]() {
        server.send(200, "application/json", _getDiagnosticDataJSON());
    });
}

String _getDiagnosticDataJSON() {
    DiagnosticData data = getDiagnosticData();
    JsonDocument doc;
    
    doc["ramUsage"] = data.ramUsage;
    doc["flashUsage"] = data.flashUsage;
    doc["uptime"] = data.uptime;
    doc["wifiConnected"] = data.wifiConnected;
    doc["rssi"] = data.rssi;
    doc["deviceTemperature"] = data.deviceTemperature;
    
    String json;
    serializeJson(doc, json);
    return json;
}