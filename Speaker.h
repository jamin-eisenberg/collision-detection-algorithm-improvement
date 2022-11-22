#ifndef PROJECT_SPEAKER_H
#define PROJECT_SPEAKER_H

#include "DE1SoCfpga.h"

const unsigned int MPCORE_PRIV_TIMER_LOAD_OFFSET = 0xDEC600;
const unsigned int MPCORE_PRIV_TIMER_COUNTER_OFFSET = 0xDEC604;
const unsigned int MPCORE_PRIV_TIMER_CONTROL_OFFSET = 0xDEC608;
const unsigned int MPCORE_PRIV_TIMER_INTERRUPT_OFFSET = 0xDEC60C;

class Speaker : public DE1SoCfpga {
private:
    unsigned int initialvalueLoadMPCore;
    unsigned int initialvalueControlMPCore;
    unsigned int initialvalueInterruptMPCore;
public:
    Speaker();

    ~Speaker();

    // plays the given tone for the given duration
    void playTone(double, int);
};


#endif
