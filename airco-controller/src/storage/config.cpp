#include <Arduino.h>
#include "config.h"

WifiCredentials getWifiCredentials() {
    // Spoof credentials for now
    return WifiCredentials{
        true,
        "test",
        "test"
    };
}

void setBootInHotspotMode(bool bootInHotspotMode) {
    // Spoof boot in ap mode for now
    // TODO: Implement
}

// BootInChangeCredentialsMode is expected to be defined before a device reboot.
bool getBootInHotspotMode() {
    // TODO: Implement
    return true;
}