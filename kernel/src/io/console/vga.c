/**
 * @file vga.c
 * @brief VGA driver implementation.
 *
 * @see http://www.osdever.net/FreeVGA/vga/vgatext.htm
 */

#include <kernel/io/console.h>
#include <kernel/io/console/vga.h>
#include <kernel/io/helper.h>
#include <string.h>

#define VGA_COLOR_FRAMEBUFFER_ADDR ((uint16_t *)0xB8000)
#define VGA_COLOR_FRAMEBUFFER_SIZE 0x8000
#define VGA_FRAMEBUFFER_WIDTH 80
#define VGA_FRAMEBUFFER_HEIGHT 25

#define VGA_CRTC_ADDR_REG 0x3D4
#define VGA_CRTC_DATA_REG 0x3D5

#define VGA_MAXIMUM_SCAN_LINE_REG 0x09
#define VGA_CURSOR_START_REG 0x0A
#define VGA_CURSOR_END_REG 0x0B
#define VGA_START_ADDR_HIGH_REG 0x0C
#define VGA_START_ADDR_LOW_REG 0x0D
#define VGA_CURSOR_LOCATION_HIGH_REG 0x0E
#define VGA_CURSOR_LOCATION_LOW_REG 0x0F

#define VGA_ERASE_CHAR (' ' | (VGA_COLOR_LIGHT_GREY << 8))

typedef enum vga_color_e {
    VGA_COLOR_BLACK         = 0,
    VGA_COLOR_BLUE          = 1,
    VGA_COLOR_GREEN         = 2,
    VGA_COLOR_CYAN          = 3,
    VGA_COLOR_RED           = 4,
    VGA_COLOR_MAGENTA       = 5,
    VGA_COLOR_BROWN         = 6,
    VGA_COLOR_LIGHT_GREY    = 7,
    VGA_COLOR_DARK_GREY     = 8,
    VGA_COLOR_LIGHT_BLUE    = 9,
    VGA_COLOR_LIGHT_GREEN   = 10,
    VGA_COLOR_LIGHT_CYAN    = 11,
    VGA_COLOR_LIGHT_RED     = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN   = 14,
    VGA_COLOR_WHITE         = 15,
} vga_color_t;

static uint16_t *g_vga_vram_start;
static uint16_t *g_vga_vram_end;
static uint8_t   g_vga_attribute;

static inline uint16_t
vga_get_pair(unsigned char c) {
    return c | g_vga_attribute << 8;
}

static void
vga_clear_vram(uint16_t *start, uint16_t *end) {
    for (uint16_t *ptr = start; ptr < end; ptr++) {
        *ptr = VGA_ERASE_CHAR;
    }
}

static void
vga_set_addr(const console_t *con) {
    uint32_t offset;

    offset = (con->viewport_visible_origin - (uintptr_t)g_vga_vram_start) / 2;

    output_byte(VGA_CRTC_ADDR_REG, VGA_START_ADDR_LOW_REG);
    output_byte(VGA_CRTC_DATA_REG, offset & 0xFF);
    output_byte(VGA_CRTC_ADDR_REG, VGA_START_ADDR_HIGH_REG);
    output_byte(VGA_CRTC_DATA_REG, (offset >> 8) & 0xFF);
}

static void
vga_set_cursor_style(const console_cursor_t cursor) {
    uint8_t cursor_scanline_start = 0;
    uint8_t cursor_scanline_end   = 0;
    uint8_t cursor_start          = 0;
    uint8_t cursor_end            = 0;

    switch (cursor) {
        case CONSOLE_CURSOR_OFF:
            cursor_scanline_start = 0;
            cursor_scanline_end   = 0;
            break;
        case CONSOLE_CURSOR_BLOCK:
            output_byte(VGA_CRTC_ADDR_REG, VGA_MAXIMUM_SCAN_LINE_REG);
            cursor_scanline_start = 0;
            cursor_scanline_end   = input_byte(VGA_CRTC_DATA_REG) & 0x0F;
            break;
        case CONSOLE_CURSOR_HALF_BLOCK:
            output_byte(VGA_CRTC_ADDR_REG, VGA_MAXIMUM_SCAN_LINE_REG);
            cursor_scanline_start = 0;
            cursor_scanline_end   = (input_byte(VGA_CRTC_DATA_REG) & 0x0F) / 2;
            break;
        case CONSOLE_CURSOR_UNDERLINE:
            cursor_scanline_start = 0;
            cursor_scanline_end   = 1;
            break;
    }

    cursor_start = (cursor == CONSOLE_CURSOR_OFF ? 0x20 : 0x00) | cursor_scanline_start;
    cursor_end   = cursor_scanline_end;

    output_byte(VGA_CRTC_ADDR_REG, VGA_CURSOR_START_REG);
    output_byte(VGA_CRTC_DATA_REG, cursor_start);
    output_byte(VGA_CRTC_ADDR_REG, VGA_CURSOR_END_REG);
    output_byte(VGA_CRTC_DATA_REG, cursor_end);
}

static bool
vga_scroll(console_t *con, console_scroll_dir_t dir, size_t n) {
    size_t delta;

    delta = n * con->viewport_row_size;
    switch (dir) {
        case CONSOLE_SCROLL_UP:
            if (con->viewport_end + delta > (uintptr_t)g_vga_vram_end) {
                /* If the visible viewport would exceed the VRAM size, wrap around to the beginning of the VRAM. */
                memcpy(g_vga_vram_start, (const void *)(con->viewport_origin + delta), con->viewport_size - delta);
                con->viewport_origin = (uintptr_t)g_vga_vram_start;
                con->viewport_end    = con->viewport_origin + con->viewport_size;
            } else {
                con->viewport_origin += delta;
                con->viewport_end += delta;
            }

            /* Clear the bottom of the viewport. */
            vga_clear_vram((uint16_t *)(con->viewport_origin + con->viewport_size - delta),
                           (uint16_t *)(con->viewport_origin + con->viewport_size));
            break;
        case CONSOLE_SCROLL_DOWN:
            /* NOT IMPLEMENTED */
            return false;
    }

    con->viewport_visible_origin = con->viewport_origin;

    /*TODO: what about the cursor position ? */

    vga_set_addr(con);

    return true;
}

static bool
vga_set_cursor_pos(const console_t *con, const size_t x, const size_t y) {
    if (x >= con->viewport_col_nbr || y >= con->viewport_row_nbr) {
        return false;
    }

    const uint16_t pos = y * con->viewport_col_nbr + x;

    output_byte(VGA_CRTC_ADDR_REG, VGA_CURSOR_LOCATION_LOW_REG);
    output_byte(VGA_CRTC_DATA_REG, pos & 0xFF);

    output_byte(VGA_CRTC_ADDR_REG, VGA_CURSOR_LOCATION_HIGH_REG);
    output_byte(VGA_CRTC_DATA_REG, (pos >> 8) & 0xFF);

    return true;
}

static bool
vga_put_char(const console_t *con, unsigned char c, size_t x, size_t y) {
    uint16_t *ptr;

    if (x >= con->viewport_col_nbr || y >= con->viewport_row_nbr) {
        return false;
    }

    ptr  = (uint16_t *)(con->viewport_origin + (y * con->viewport_row_size + x * con->viewport_col_size));
    *ptr = vga_get_pair(c);

    return true;
}

static void
vga_init(console_t *con) {
    g_vga_vram_start = VGA_COLOR_FRAMEBUFFER_ADDR;
    g_vga_vram_end   = g_vga_vram_start + (VGA_FRAMEBUFFER_WIDTH * VGA_FRAMEBUFFER_HEIGHT) * 2;
    g_vga_attribute  = VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4;

    vga_clear_vram(g_vga_vram_start, g_vga_vram_end);

    con->viewport_col_size = 2;
    con->viewport_col_nbr  = VGA_FRAMEBUFFER_WIDTH;

    con->viewport_row_nbr  = VGA_FRAMEBUFFER_HEIGHT;
    con->viewport_row_size = con->viewport_col_nbr * con->viewport_col_size;

    con->viewport_size = con->viewport_row_nbr * con->viewport_row_size;

    con->viewport_origin         = (uintptr_t)g_vga_vram_start;
    con->viewport_visible_origin = con->viewport_origin;
    con->viewport_end            = con->viewport_origin + con->viewport_size;
}

console_impl_t VGA_CONSOLE_DRIVER = {
    .init             = vga_init,
    .put_char         = vga_put_char,
    .scroll           = vga_scroll,
    .set_cursor_pos   = vga_set_cursor_pos,
    .set_cursor_style = vga_set_cursor_style,
};