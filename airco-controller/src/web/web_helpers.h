#pragma once

#include <WebServer.h>

void serveFile(WebServer& server, const char* path, const char* contentType);