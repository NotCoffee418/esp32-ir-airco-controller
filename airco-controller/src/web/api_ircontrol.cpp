#include "api_ircontrol.h"
#include "authorization.h"
#include "modules/ir_remote.h"
#include "modules/mode_switch.h"
#include "web/web_helpers.h"

void registerIrControlApiHandlers(WebServer& server) {
    server.on("/api/ir/off", [&server]() {
        if (!authorizeWebOrApiHandler(server)) {
            return;
        }

        // Return false without error if mode is manual
        if (!isModeSwitchOnAuto()) {
            apiSuccessResp(server, false);
            return;
        }
        
        turnOffAC();
        apiSuccessResp(server, true);
    });

    server.on("/api/ir/on", [&server]() {
        if (!authorizeWebOrApiHandler(server)) {
            return;
        }
        if (!requireBody(server)) {
            return;
        }

        // Return false without error if mode is manual
        if (!isModeSwitchOnAuto()) {
            apiSuccessResp(server, false);
            return;
        }

        // Parse the JSON body
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, server.arg("plain"));
        if (error) {
            apiErrorResp(server, "Invalid JSON", 400);
            return;
        }


        // Mode
        if (!doc.containsKey("mode")) {
            apiErrorResp(server, "mode is required in the body", 400);
            return;
        }
        if (!doc["mode"].is<int>()) {
            apiErrorResp(server, "mode must be an integer", 400);
            return;
        }
        int mode = doc["mode"].as<int>();
        if (mode < 1 || mode > 4) {
            apiErrorResp(server, "mode must be between 0 and 4", 400);
            return;
        }
        AcMode acMode = static_cast<AcMode>(mode);

        // Fan speed
        if (!doc.containsKey("fanSpeed")) {
            apiErrorResp(server, "fanSpeed is required in the body", 400);
            return;
        }
        if (!doc["fanSpeed"].is<int>()) {
            apiErrorResp(server, "fanSpeed must be an integer", 400);
            return;
        }
        int fanSpeed = doc["fanSpeed"].as<int>();
        if (fanSpeed < 0 || fanSpeed > 4) {
            apiErrorResp(server, "fanSpeed must be between 0 and 4", 400);
            return;
        }
        FanSpeed acFanSpeed = static_cast<FanSpeed>(fanSpeed);

        // Swing
        if (!doc.containsKey("swing")) {
            apiErrorResp(server, "swing is required in the body", 400);
            return;
        }
        if (!doc["swing"].is<int>()) {
            apiErrorResp(server, "swing must be an integer", 400);
            return;
        }
        int swing = doc["swing"].as<int>();
        if (swing < 0 || swing > 1) {
            apiErrorResp(server, "swing must be between 0 and 1", 400);
            return;
        }
        AcSwing acSwing = static_cast<AcSwing>(swing);

        // Temperature
        if (!doc.containsKey("temperature")) {
            apiErrorResp(server, "temperature is required in the body", 400);
            return;
        }
        if (!doc["temperature"].is<float>() && !doc["temperature"].is<int>()) {
            apiErrorResp(server, "temperature must be a number", 400);
            return;
        }
        float temperature = doc["temperature"].as<float>();
        if (acMode == AcMode::AC_MODE_COOL) {
            if (temperature < 18.0f || temperature > 30.0f) {
                apiErrorResp(server, "temperature must be between 16.0 and 30.0", 400);
                return;
            }
        } else if (acMode == AcMode::AC_MODE_HEAT) {
            if (temperature < 16.0f || temperature > 30.0f) {
                apiErrorResp(server, "temperature must be between 16.0 and 30.0", 400);
                return;
            }
        }
        
        turnOnAC(acMode, acFanSpeed, temperature, acSwing);
        apiSuccessResp(server, true);
    });

    server.on("/api/ir/toggle-powerful", [&server]() {
        if (!authorizeWebOrApiHandler(server)) {
            return;
        }

        // Return false without error if mode is manual
        if (!isModeSwitchOnAuto()) {
            apiSuccessResp(server, false);
            return;
        }

        togglePowerful();        
        apiSuccessResp(server, true);
    });

}