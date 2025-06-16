#include "api_sensors.h"
#include "modules/mode_switch.h"
#include "modules/temperature.h"
#include "web/web_helpers.h"


void registerSensorsApiHandlers(WebServer& server) {
    server.on("/api/sensors", [&server]() {
        // Manual, apiSuccessResp doesn't like struct
        JsonDocument doc;
        doc["success"] = true;
        doc["error"] = "";
        doc["data"]["temperature"] = getCachedTempC();
        doc["data"]["isAutomaticMode"] = isModeSwitchOnAuto();

        String output;
        serializeJson(doc, output);
        server.send(200, "application/json", output);
    });
}