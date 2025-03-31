#ifndef _VGA_H
#define _VGA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_TERMINAL_WIDTH 80
#define VGA_TERMINAL_HEIGHT 25

#define VGA_CRTC_ADDR_REG 0x3D4
#define VGA_CRTC_DATA_REG 0x3D5

#define VGA_MAXIMUM_SCAN_LINE_REG 0x09
#define VGA_CURSOR_START_REG 0x0A
#define VGA_CURSOR_END_REG 0x0B
#define VGA_START_ADDR_HIGH_REG 0x0C
#define VGA_START_ADDR_LOW_REG 0x0D
#define VGA_CURSOR_LOCATION_HIGH_REG 0x0E
#define VGA_CURSOR_LOCATION_LOW_REG 0x0F

typedef enum vga_color_e
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color_t;

typedef enum vga_cursor_e
{
    VGA_CURSOR_OFF,
    VGA_CURSOR_UNDERLINE,
    VGA_CURSOR_BLOCK,
} vga_cursor_t;

typedef enum vga_scroll_dir_e {
    VGA_SCROLL_UP,
    VGA_SCROLL_DOWN,
} vga_scroll_dir_t;

void vga_init();
void vga_set_attribute(vga_color_t fg, vga_color_t bg);
void vga_set_cursor_style(vga_cursor_t cursor);

bool vga_scroll(vga_scroll_dir_t dir, size_t n);
bool vga_scrolldelta(vga_scroll_dir_t dir, size_t n);
bool vga_set_cursor_position(size_t x, size_t y);
bool vga_put_char(unsigned char c, size_t x, size_t y);

#endif