#include "VGATerminal.hpp"

VGATerminal::VGATerminal() {
    /* Clear screen buffer. */
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            buffer[y * WIDTH + x] = 0x0020;
        }
    }
}

void VGATerminal::putString(const char *str, Color fg, Color bg) {
    while (*str) {
        putChar(*str, fg, bg);
        str++;
    }
}

void VGATerminal::putChar(unsigned char c, Color fg, Color bg) {
    const auto attribute{static_cast<uint8_t>(fg | bg << 4)};

    if (c == '\n') {
        column = 0;
        row++;
        return;
    }

    buffer[row * WIDTH + column] = c | attribute << 8;

    if (++column == WIDTH) {
        column = 0;
        row++;
    }
}