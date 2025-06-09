#include "api_ircontrol.h"
#include "authorization.h"
#include "modules/ir_remote.h"

void registerIrControlApiHandlers(WebServer& server) {
    server.on("/api/ir/off", [&server]() {
        // if (!authorizeHandler(server)) {
        //     return;
        // }
        turnOffAC();
        server.send(200, "text/plain", "AC turned off at " + String(millis()));
    });

    server.on("/api/ir/on", [&server]() {
        // if (!authorizeHandler(server)) {
        //     return;
        // }
        turnOnAC();
        server.send(200, "text/plain", "AC turned on at " + String(millis()));
    });

}