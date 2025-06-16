#pragma once

#include <Arduino.h>
#include <WebServer.h>

bool authorizeWebHandler(WebServer& server, bool redirectAndReset = true);
bool authorizeApiHandler(WebServer& server);
bool authorizeWebOrApiHandler(WebServer& server);
void login(WebServer& server, String inputPin);