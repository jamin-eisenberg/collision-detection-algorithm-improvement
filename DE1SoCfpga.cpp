#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <sys/mman.h>
#include <iostream>

#include "DE1SoCfpga.h"

using namespace std;

// maps memory to be accessed
DE1SoCfpga::DE1SoCfpga(int bridgeBase, int bridgeSpan) {
    fd = open("/dev/mem", (O_RDWR | O_SYNC));
    if (fd == -1) // check for errors in openning /dev/mem
    {
        cout << "ERROR: could not open /dev/mem..." << endl;
        exit(1);
    }
    // Get a mapping from physical addresses to virtual addresses
    pBase = (char *) mmap(NULL, bridgeSpan, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, bridgeBase);
    if (pBase == MAP_FAILED) // check for errors
    {
        cout << "ERROR: mmap() failed..." << endl;
        close(fd); // close memory before exiting
        exit(1); // Returns 1 to the operating system;
    }
}

DE1SoCfpga::~DE1SoCfpga() {
    if (munmap(pBase, LW_BRIDGE_SPAN) != 0) {
        cout << "ERROR: munmap() failed..." << endl;
        exit(1);
    }
    close(fd); // close memory
}

// writes the given value to the given mapped address
void DE1SoCfpga::RegisterWrite(unsigned intoffset, int value) {
    *(volatile unsigned int *) (pBase + intoffset) = value;
}

// reads the value at the given mapped address
int DE1SoCfpga::RegisterRead(unsigned intoffset) {
    return *(volatile unsigned int *) (pBase + intoffset);
}

