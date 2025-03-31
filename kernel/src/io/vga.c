/**
 * @file vga.c
 * @brief VGA driver implementation
 * 
 * @see http://www.osdever.net/FreeVGA/vga/vgatext.htm
 */

#include <kernel/io/helper.h>
#include <kernel/io/vga.h>
#include <string.h>
#include "vga.h"

#define VGA_COLOR_FRAMEBUFFER_ADDR ((uint16_t *)0xB8000)
#define VGA_FRAMEBUFFER_WIDTH 80
#define VGA_FRAMEBUFFER_HEIGHT 25

#define VGA_ERASE_CHAR (' ' | (VGA_COLOR_LIGHT_GREY << 8))

static uint16_t *g_vga_vram_start;
static uint16_t *g_vga_vram_end;
static uint8_t   g_vga_attribute;

static uint16_t *g_screen_visible_origin;
static uint16_t *g_screen_origin;
static uint16_t *g_screen_pos;
static uint16_t *g_screen_end;
static size_t    g_screen_size;

static inline uint16_t vga_get_pair(unsigned char c) {
    return c | g_vga_attribute << 8;
}

static void
vga_clear_vram() {
    for (uint16_t *ptr = g_vga_vram_start; ptr < g_vga_vram_end; ptr++) {
        *ptr = VGA_ERASE_CHAR;
    }
}

static void vga_set_addr() {
}

void
vga_set_cursor_style(const vga_cursor_t cursor) {
    uint8_t cursor_scanline_start = 0;
    uint8_t cursor_scanline_end   = 0;
    uint8_t cursor_start          = 0;
    uint8_t cursor_end            = 0;

    switch (cursor) {
        case VGA_CURSOR_OFF:
            cursor_scanline_start = 0;
            cursor_scanline_end   = 0;
            break;
        case VGA_CURSOR_BLOCK:
            output_byte(VGA_CRTC_ADDR_REG, VGA_MAXIMUM_SCAN_LINE_REG);
            cursor_scanline_start = 0;
            cursor_scanline_end   = input_byte(VGA_CRTC_DATA_REG) & 0x0F;
            break;
        case VGA_CURSOR_UNDERLINE:
            cursor_scanline_start = 0;
            cursor_scanline_end   = 1;
            break;
    }

    cursor_start = (cursor == VGA_CURSOR_OFF ? 0x20 : 0x00) | cursor_scanline_start;
    cursor_end   = cursor_scanline_end;

    output_byte(VGA_CRTC_ADDR_REG, cursor_start);
    output_byte(VGA_CRTC_DATA_REG, cursor_end);
}

bool
vga_scroll(vga_scroll_dir_t dir, size_t n) {
    uint16_t *old_origin = g_screen_origin;

    switch (dir) {
        case VGA_SCROLL_UP:
        break;
        case VGA_SCROLL_DOWN:
        break;
    }

    return false;
}

bool
vga_set_cursor_position(const size_t x, const size_t y) {
    if (x >= VGA_FRAMEBUFFER_WIDTH || y >= VGA_FRAMEBUFFER_HEIGHT) {
        return false;
    }

    const uint16_t pos = y * VGA_FRAMEBUFFER_WIDTH + x;

    output_byte(VGA_CRTC_ADDR_REG, VGA_CURSOR_LOCATION_LOW_REG);
    output_byte(VGA_CRTC_DATA_REG, pos & 0xFF);

    output_byte(VGA_CRTC_ADDR_REG, VGA_CURSOR_LOCATION_HIGH_REG);
    output_byte(VGA_CRTC_DATA_REG, (pos >> 8) & 0xFF);
}

bool
vga_put_char(unsigned char c, size_t x, size_t y) {
    if (x >= VGA_FRAMEBUFFER_WIDTH || y >= VGA_FRAMEBUFFER_HEIGHT) {
        return false;
    }

    g_screen_origin[y * VGA_FRAMEBUFFER_WIDTH + x] = vga_get_pair(c);
}

void
vga_init() {
    g_vga_vram_start = VGA_COLOR_FRAMEBUFFER_ADDR;
    g_vga_vram_end   = g_vga_vram_start + (VGA_FRAMEBUFFER_WIDTH * VGA_FRAMEBUFFER_HEIGHT * 2);
    g_vga_attribute  = VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4;

    vga_clear_vram();

    g_screen_origin = g_screen_visible_origin = g_vga_vram_start;
    g_screen_size                             = (uint8_t *)g_vga_vram_end - (uint8_t *)g_vga_vram_start;
    g_screen_end                              = g_screen_origin + g_screen_size;
    g_screen_pos                              = g_screen_origin;
}

void
vga_set_attribute(vga_color_t fg, vga_color_t bg) {
    g_vga_attribute = fg | bg << 4;
}