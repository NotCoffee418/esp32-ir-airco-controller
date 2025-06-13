#include <Arduino.h>

#define MODE_SWITCH_PIN 12

void setupModeSwitch() {
    pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);
}

bool isModeSwitchOnAuto() {
    // HIGH = off, LOW = on
    // Off = manual, On = auto
    return digitalRead(MODE_SWITCH_PIN) == LOW;
}