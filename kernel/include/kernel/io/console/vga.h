#ifndef _VGA_H
#define _VGA_H

#include <kernel/io/console.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static inline uint8_t
vga_get_attr(console_color_t fg, console_color_t bg) {
    return fg | bg << 4;
}

static inline uint16_t
vga_get_pair(unsigned char c, uint8_t attr) {
    return c | attr << 8;
}

extern console_impl_t VGA_CONSOLE_DRIVER;

#endif