#include <ir_Fujitsu.h>

#define IR_SEND_PIN 4

IRFujitsuAC ac(IR_SEND_PIN);

void setupIrRemote() {
    ac.begin();

    // For actual model: 
    // AC living: ASHG24KMTA (living room)
    // Remote living (0.5 degree steps): AR-REW2E
    // Remote veranda (1 degree steps): AR-REB1E
    ac.setModel(fujitsu_ac_remote_model_t::ARREB1E);
}

void turnOffAC() {
    ac.off();
    ac.send();

    Serial.println("AC turned OFF");
}

void turnOnAC() {
    ac.on();
    ac.send();

    // Cannot combine settings with on signal, wait 2s
    delay(2000);


    ac.setTemp(24);
    ac.setMode(kFujitsuAcModeCool);
    ac.setFanSpeed(kFujitsuAcFanAuto);

    ac.send();
}