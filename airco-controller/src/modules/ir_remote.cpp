#include <ir_Fujitsu.h>

#define IR_SEND_PIN 4

IRFujitsuAC ac(IR_SEND_PIN);

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

void turnOffAC() {
    ac.off();
    ac.send();

    Serial.println("AC turned OFF");
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
void turnOnAC() { 
    ac.setCmd(FUJITSU_AC_CMD_TURN_ON);
    ac.send();

    // Cannot combine settings with on signal, wait 2s
    delay(2000);

    ac.setSwing(FUJITSU_AC_SWING_VERT);
    ac.setMode(FUJITSU_AC_MODE_COOL);
    ac.setFanSpeed(FUJITSU_AC_FAN_AUTO);
    ac.setTemp(24);

    ac.send();
}