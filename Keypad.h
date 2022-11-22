#ifndef KEY_H
#define KEY_H

#include "DE1SoCfpga.h"

const unsigned int DIRECTION_REG_ADDR = DATA_REG_ADDR + 4;

const unsigned int COL_VALS[] = {14, 13, 11, 9};
const unsigned int ROW_VALS[] = {21, 19, 17, 15};

// represents a 4x4 external keypad
class Keypad : public DE1SoCfpga {
public:
    Keypad();

    ~Keypad();

    // gets the key being currently pressed
    int read();
};

#endif
