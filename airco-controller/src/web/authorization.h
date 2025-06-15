#pragma once

#include <Arduino.h>
#include <WebServer.h>

bool authorizeWebHandler(WebServer& server);
bool authorizeApiHandler(WebServer& server);
void login(WebServer& server, String inputPin);