#include <ir_Fujitsu.h>

#define IR_SEND_PIN 4

IRFujitsuAC ac(IR_SEND_PIN);
volatile bool _irRemoteIsSending = false;

void setupIrRemote() {
    ac.begin();

    // For actual model: 
    // AC living: ASHG24KMTA (living room)
    // AC veranda: ASHG09KMCC (veranda)
    // Remote living (0.5 degree steps): AR-REW2E
    // Remote veranda (1 degree steps): AR-REB1E

    // Misplaced notes:
    // use FUJITSU_AC_CMD_TURN_ON for both ACs
    // Order seems to matter if we want to send combined command:
    // on, swing, mode, speed, temp
    ac.setModel(fujitsu_ac_remote_model_t::ARREB1E); // 
}

bool turnOffAC() {
    if (_irRemoteIsSending) {
        return false;
    }
    _irRemoteIsSending = true;

    // Small delay to let ESP32 settle background tasks
    delay(100);

    // Turn off AC
    ac.off();

    portDISABLE_INTERRUPTS();
    ac.send();
    portENABLE_INTERRUPTS();

    Serial.println("AC turned OFF");

    _irRemoteIsSending = false;
    return true;
}

// Power on AC and define settings. 
// Cannot send in one command or one of the ACs refuses to work on both ACs.
// Hours of testing various combinations: 3
//
// Order of commands should be as described in this comment.
// Not 100% sure it makes a difference but it was wobbly during a test run otherwise.
// Enabling to work for both must be done with:
// - setCmd(FUJITSU_AC_CMD_TURN_ON)
// Delay:
// - 2000 works, two beeps
// - 1000 untested, idk if we can get it to 1 beep but github people claim 1000 should work
// Swing: 
// - FUJITSU_AC_SWING_VERT works, 
// - undefined works (not sure if it retains old settings)
// - FUJITSU_AC_SWING_HORIZ second AC doesnt support, didnt test
// - FUJITSU_AC_SWING_OFF untested
// Temp: 
// - Theoretically x.5 works, not tested
// Mode:
// - COOL works
// - Other modes untested
// Fan:
// - AUTO works
// - Other speeds untested
bool turnOnAC() { 
    if (_irRemoteIsSending) {
        return false;
    }
    _irRemoteIsSending = true;

    // Small delay to let ESP32 settle background tasks
    delay(100);

    // Turn on AC
    ac.setCmd(FUJITSU_AC_CMD_TURN_ON);

    portDISABLE_INTERRUPTS();
    ac.send();
    portENABLE_INTERRUPTS();

    // Cannot combine settings with on signal, wait 2s
    delay(2000);

    // Change settings
    ac.setSwing(FUJITSU_AC_SWING_VERT);
    ac.setMode(FUJITSU_AC_MODE_COOL);
    ac.setFanSpeed(FUJITSU_AC_FAN_AUTO);
    ac.setTemp(24);

    portDISABLE_INTERRUPTS();
    ac.send();
    portENABLE_INTERRUPTS();

    _irRemoteIsSending = false;
    return true;
}