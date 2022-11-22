

#ifndef PROJECT_SCREEN_H
#define PROJECT_SCREEN_H

#include "DE1SoCfpga.h"

//const unsigned int BASE = 0xC0000000;
const unsigned int SPAN = 0x4B000;

const unsigned int PIXEL_BUF_CTRL_BASE = 0x3020;
const unsigned int RGB_RESAMPLER_BASE = 0x3010;

// represents the screen the VGA port draws to
class Screen {
private:
    char *pBase;
    int fd;

    int screen_x;
    int res_offset;
    int col_offset;
    int x_factor, y_factor;

    int speaker;

    short white;

    int resample_rgb(int, int);
    int get_data_bits(int);

public:
    Screen(DE1SoCfpga*);

    ~Screen();

    // writes black or white (bool) at the given pixel (x, y)
    void writePixel(int, int, bool);

    // Uses buffer swapping outlined in the chip’s manual
    // Waits until it’s time to update (60 Hz) using the timing feature outlined in the manual
    void update(DE1SoCfpga*);

    // writes every pixel black
    void clear();
};

#endif
