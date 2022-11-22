#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>

#include "Screen.h"
#include "DE1SoCfpga.h"

using namespace std;

Screen::Screen(DE1SoCfpga *fpga) {
    fd = open("/dev/mem", (O_RDWR | O_SYNC));
    if (fd == -1) // check for errors in opening /dev/mem
    {
        cout << "ERROR: could not open /dev/mem..." << endl;
        exit(1);
    }
    // Get a mapping from physical addresses to virtual addresses
    pBase = (char *) mmap(NULL, SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, fpga->RegisterRead(PIXEL_BUF_CTRL_BASE + 0x4));
    if (pBase == MAP_FAILED) // check for errors
    {
        cout << "ERROR: mmap() failed..." << endl;
        close(fd); // close memory before exiting
        exit(1); // Returns 1 to the operating system;
    }

    int res = fpga->RegisterRead(PIXEL_BUF_CTRL_BASE + 0x8);

    screen_x = res & 0xFFFF;

    int db = get_data_bits(fpga->RegisterRead(RGB_RESAMPLER_BASE) & 0x3F);
/* check if resolution is smaller than the standard 320 x 240 */
    res_offset = (screen_x == 160) ? 1 : 0;
/* check if number of data bits is less than the standard 16-bits */
    col_offset = (db == 8) ? 1 : 0;

    x_factor = 0x1 << (res_offset + col_offset);
    y_factor = 0x1 << (res_offset);

    white = resample_rgb(db, 0xFFFFFF);

    speaker = 0;
}

Screen::~Screen() {
    if (munmap(pBase, SPAN) != 0) {
        cout << "ERROR: munmap() failed..." << endl;
        exit(1);
    }
    close(fd); // close memory
}

int Screen::resample_rgb(int num_bits, int color) {
    if (num_bits == 8) {
        color = (((color >> 16) & 0x000000E0) | ((color >> 11) & 0x0000001C) |
                 ((color >> 6) & 0x00000003));
        color = (color << 8) | color;
    } else if (num_bits == 16) {
        color = (((color >> 8) & 0x0000F800) | ((color >> 5) & 0x000007E0) |
                 ((color >> 3) & 0x0000001F));
    }
    return color;
}

int Screen::get_data_bits(int mode) {
    switch (mode) {
        case 0x0:
            return 1;
        case 0x7:
            return 8;
        case 0x11:
            return 8;
        case 0x12:
            return 9;
        case 0x14:
            return 16;
        case 0x17:
            return 24;
        case 0x19:
            return 30;
        case 0x31:
            return 8;
        case 0x32:
            return 12;
        case 0x33:
            return 16;
        case 0x37:
            return 32;
        case 0x39:
            return 40;
        default:
            return -1;
    }
}

// writes black or white (bool) at the given pixel (x, y)
void Screen::writePixel(int x, int y, bool isWhite) {
    // Uses the operations in the “Video” example found in the manual

    // if within bounds
    if(x >= 0 && x < 320 && y >= 0 && y < 240) {

        x /= x_factor;
        y /= y_factor;

        int pixel_ptr = (y << (10 - res_offset - col_offset)) + (x << 1);
        *(volatile unsigned int *) (pBase + pixel_ptr) = (isWhite ? white : 0);
    }
}

// Uses buffer swapping outlined in the chip’s manual
// Waits until it’s time to update (60 Hz) using the timing feature outlined in the manual
void Screen::update(DE1SoCfpga *fpga) {
    while((fpga->RegisterRead(PIXEL_BUF_CTRL_BASE + 0xC) & 1) == 1){}
    fpga->RegisterWrite(PIXEL_BUF_CTRL_BASE, 1);
    fpga->RegisterWrite(0x64, 1);
    speaker = ~speaker;
    fpga->RegisterWrite(0x60, speaker);
}

// writes every pixel black
void Screen::clear() {
    for(int y = 0; y < 240; y++){
        for(int x = 0; x < 320; x++){
            writePixel(x, y, false);
        }
    }
}

