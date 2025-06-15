#include <Arduino.h>
#include <WebServer.h>

#include "network/device_identity.h"
#include "authorization.h"

// Constants
static const uint8_t _sessionTokenLength = 16;
static const unsigned long _maxTokenAgeMs = 1800000; // 30 min


// private variables
static String _activeSessionToken = "";
static unsigned long _activeSessionTokenCreatedAt = 0;

// Prviate functions
bool _hasActiveSessionCookie(WebServer& server);

bool authorizeWebHandler(WebServer& server) {
    // Check if user has valid session cookie
    if (_hasActiveSessionCookie(server)) {
        return true;
    }

    // Does not have valid session cookie, redirect to login
    // Clear cookie if any
    server.sendHeader("Location", "/login");
    server.sendHeader("Set-Cookie", "session=; Path=/; Max-Age=0");
    server.send(302, "text/plain", "Not logged in");
    return false;
}

void login(WebServer& server, String inputPin) {
    // If user is already logged in, redirect to configure
    if (_hasActiveSessionCookie(server)) {
        server.sendHeader("Location", "/configure");
        server.send(302, "text/plain", "Already logged in");
        return;
    }

    // Validate input
    if (inputPin.length() > 32) {
        server.send(400, "text/plain", "Pin too long");
        return;
    }
    inputPin.toUpperCase();
    inputPin.trim();

    // Grant cookie with access pin or redirect to /login with error
    String accessPin = getAccessPin();
    if (inputPin == accessPin) {
        _activeSessionToken = generateRandomString(_sessionTokenLength);
        _activeSessionTokenCreatedAt = millis();
        server.sendHeader("Set-Cookie", "session=" + _activeSessionToken + "; Path=/; Max-Age=" + String(_maxTokenAgeMs / 1000));
        server.sendHeader("Location", "/configure");
        server.send(302, "text/plain", "Login successful");
        return;
    } else {
        server.sendHeader("Location", "/login?failed=true");
        server.send(302, "text/plain", "Login failed");
    }
}

bool _hasActiveSessionCookie(WebServer& server) {
    // Check if active session is expired (global)
    if (_activeSessionTokenCreatedAt + _maxTokenAgeMs < millis()) {
        _activeSessionToken = "";
        _activeSessionTokenCreatedAt = 0;
        return false;
    }
    
    // Check cookie (user)
    String session = server.header("Cookie");
    if (session.length() > 0 && session.indexOf("session=") != -1) {
        uint8_t sessionStartIdx = session.indexOf("session=") + 8;
        uint8_t sessionEndIdx = sessionStartIdx + _sessionTokenLength;
        String sessionToken = session.substring(sessionStartIdx, sessionEndIdx);
        if (sessionToken == _activeSessionToken) {
            return true;
        }
    }

    // No session cookie or invalid/overridden session token
    return false;
}