#pragma once

#include <IRremoteESP8266.h>
#include <ir_Fujitsu.h>

enum class FanSpeed : uint8_t {
    AC_SPEED_AUTO = kFujitsuAcFanAuto,     // 0x00
    AC_SPEED_HIGH = kFujitsuAcFanHigh,     // 0x01
    AC_SPEED_MED = kFujitsuAcFanMed,       // 0x02
    AC_SPEED_LOW = kFujitsuAcFanLow,       // 0x03
    AC_SPEED_QUIET = kFujitsuAcFanQuiet    // 0x04
};

enum class AcMode : uint8_t {
    // AC_MODE_AUTO = kFujitsuAcModeAuto,     // 0x00 - supported but not recommended
    AC_MODE_COOL = kFujitsuAcModeCool,     // 0x01
    AC_MODE_DRY = kFujitsuAcModeDry,       // 0x02
    AC_MODE_FAN = kFujitsuAcModeFan,       // 0x03
    AC_MODE_HEAT = kFujitsuAcModeHeat      // 0x04
};

enum class AcSwing : uint8_t {
    AC_SWING_OFF = kFujitsuAcSwingOff,     // 0x00
    AC_SWING_VERT = kFujitsuAcSwingVert,   // 0x01
    // AC_SWING_HORIZ = kFujitsuAcSwingHoriz, // 0x02 - not supported by second AC
    //AC_SWING_BOTH = kFujitsuAcSwingBoth    // 0x03 - not supported by second AC
};

void setupIrRemote();
bool turnOffAC();
bool turnOnAC(AcMode mode, FanSpeed fanSpeed, float tempC, AcSwing swing, bool powerful);
bool setPowerful(bool isPowerful);