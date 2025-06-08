#pragma once

#include <Arduino.h>
#include <WebServer.h>

bool authorizeHandler(WebServer& server);
void login(WebServer& server, String inputPin);
bool hasActiveSessionCookie(WebServer& server);