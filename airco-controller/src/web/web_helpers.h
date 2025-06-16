#pragma once

#include <WebServer.h>
#include <ArduinoJson.h>

void serveFile(WebServer& server, const char* path, const char* contentType);
bool requirePost(WebServer& server);
bool requireGet(WebServer& server);
bool requireBody(WebServer& server);

template<typename T>
struct Response {
    T data;
    String error;
    bool success;
};

template<typename T>
void apiSuccessResp(WebServer& server, T data) {
    JsonDocument doc;
    doc["data"] = data;
    doc["error"] = "";
    doc["success"] = true;
    
    String output;
    serializeJson(doc, output);
    server.send(200, "application/json", output);
}

// Add this function:
inline void apiErrorResp(WebServer& server, String error, int statusCode = 200) {
    JsonDocument doc;
    doc["data"] = nullptr;
    doc["error"] = error;
    doc["success"] = false;
    
    String output;
    serializeJson(doc, output);
    server.send(statusCode, "application/json", output);
}

// Specialization for std::vector<String>
template<>
inline void apiSuccessResp<std::vector<String>>(WebServer& server, std::vector<String> data) {
    JsonDocument doc;
    
    JsonArray dataArray = doc["data"].to<JsonArray>();
    for (const String& item : data) {
        dataArray.add(item);
    }
    
    doc["error"] = "";
    doc["success"] = true;
    
    String output;
    serializeJson(doc, output);
    server.send(200, "application/json", output);
}