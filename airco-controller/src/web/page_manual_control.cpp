#include "page_manual_control.h"
#include "web/web_helpers.h"
#include "authorization.h"

void registerManualControlHandlers(WebServer& server) {
    server.on("/control", [&server]() {
        if (!authorizeWebHandler(server, true)) {
            return;
        }

        serveFile(server, "/web/manual_control.html", "text/html");
    });
}