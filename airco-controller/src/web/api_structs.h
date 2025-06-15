// These functions need to be in header file because template.
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

template<typename T>
struct Response {
    T data;
    String error;
    bool success;
};

#pragma once
#include <ArduinoJson.h>

template<typename T>
String apiSuccessResp(T data) {
    JsonDocument doc;
    doc["data"] = data;
    doc["error"] = "";
    doc["success"] = true;
    
    String output;
    serializeJson(doc, output);
    return output;
}

// Add this function:
String apiErrorResp(String error) {
    JsonDocument doc;
    doc["data"] = nullptr;
    doc["error"] = error;
    doc["success"] = false;
    
    String output;
    serializeJson(doc, output);
    return output;
}

// Specialization for std::vector<String>
template<>
String apiSuccessResp<std::vector<String>>(std::vector<String> data) {
    JsonDocument doc;
    
    JsonArray dataArray = doc["data"].to<JsonArray>();
    for (const String& item : data) {
        dataArray.add(item);
    }
    
    doc["error"] = "";
    doc["success"] = true;
    
    String output;
    serializeJson(doc, output);
    return output;
}