#include "web_helpers.h"
#include "authorization.h"

void registerLoginPageHandlers(WebServer& server) {
    server.on("/login", [&server]() {
        if (server.method() == HTTP_GET) {
            serveFile(server, "/web/login.html", "text/html");
        } else if (server.method() == HTTP_POST) {
            String inputPin = server.arg("accessPin");
            login(server, inputPin);
        }
    });
}