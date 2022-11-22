#include "Speaker.h"
#include <iostream>

using namespace std;

Speaker::Speaker(){
    initialvalueLoadMPCore = RegisterRead(MPCORE_PRIV_TIMER_LOAD_OFFSET);
    initialvalueControlMPCore = RegisterRead(MPCORE_PRIV_TIMER_CONTROL_OFFSET);
    initialvalueInterruptMPCore = RegisterRead(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET);
}

Speaker::~Speaker() {
    RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, initialvalueLoadMPCore);
    RegisterWrite(MPCORE_PRIV_TIMER_CONTROL_OFFSET, initialvalueControlMPCore);
    RegisterWrite(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, initialvalueInterruptMPCore);
}

// plays the given tone for the given duration
void Speaker::playTone(double duration, int hz) {
    int lowcount = (int) (200000000 / hz) / 2;

    RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, lowcount);

    int entervalue = 0;
    int half_wave_count = duration * hz;

    while (half_wave_count > 0 && (RegisterRead(SW_OFFSET) & 1) != 0){
        if (RegisterRead(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET) != 0) {
            cout << half_wave_count << endl;
            RegisterWrite(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, 1); // reset timer flag
            entervalue = entervalue^0x1;
            RegisterWrite(DATA_REG_ADDR, entervalue);

            half_wave_count--;
        }
    }
}