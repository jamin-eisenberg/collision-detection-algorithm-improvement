#ifndef DE1SOCFPGA_H
#define DE1SOCFPGA_H

const unsigned int LW_BRIDGE_BASE = 0xFF200000;
const unsigned int LW_BRIDGE_SPAN = 0x00DEC700;
const unsigned int SW_OFFSET = 64;

const unsigned int DATA_REG_ADDR = 0x60;

// represents a memory mapping that can be used to get values "built into" the board
class DE1SoCfpga {
public:
    char *pBase;
    int fd;

    // maps memory to be accessed
    DE1SoCfpga(int bridgeBase = LW_BRIDGE_BASE, int bridgeSpan = LW_BRIDGE_SPAN);

    ~DE1SoCfpga();

    // writes the given value to the given mapped address
    void RegisterWrite(unsigned intoffset, int value);

    // reads the value at the given mapped address
    int RegisterRead(unsigned intoffset);
};

#endif
