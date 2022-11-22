#include "Keypad.h"

#include <iostream>

using namespace std;

Keypad::Keypad() = default;

Keypad::~Keypad() = default;

// gets the key being currently pressed
int Keypad::read() {
    RegisterWrite(DIRECTION_REG_ADDR, ~0x6A00);

    int data[4][4];

    for (int row = 0; row < 4; row++) {
        RegisterWrite(DATA_REG_ADDR, 1 << ROW_VALS[row]);
        for (int col = 0; col < 4; col++) {
            data[row][col] = ((unsigned int) RegisterRead(DATA_REG_ADDR) >> COL_VALS[col]) & 1;
        }
    }

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (data[row][col] == 0) {
                for (int i = 0; i < 4; i++) {
                    if (data[i][col] == 1) {
                        return 4 * i + col + 1;
                    }
                }
            }
        }
    }

    return 0;
}

