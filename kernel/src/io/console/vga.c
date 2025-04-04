/**
 * @file vga.c
 * @brief VGA driver implementation.
 *
 * @see http://www.osdever.net/FreeVGA/vga/vgatext.htm
 * @see http://www.osdever.net/FreeVGA/vga/crtcreg.htm
 */

#include <kernel/io/console.h>
#include <kernel/io/console/vga.h>
#include <kernel/io/helper.h>
#include <string.h>

#define VGA_FRAMEBUFFER_WIDTH 80
#define VGA_FRAMEBUFFER_HEIGHT 25

#define VGA_COLOR_FRAMEBUFFER_ADDR 0xB8000
#define VGA_COLOR_VISIBLE_PAGE 8

#if VGA_COLOR_VISIBLE_PAGE > 8
#error "VGA Color framebuffer is only 8 pages big.".
#endif

#define VGA_COLOR_FRAMEBUFFER_SIZE ((VGA_FRAMEBUFFER_WIDTH * 2 * VGA_FRAMEBUFFER_HEIGHT) * VGA_COLOR_VISIBLE_PAGE)

#define VGA_CRTC_ADDR_REG 0x3D4
#define VGA_CRTC_DATA_REG 0x3D5

#define VGA_MAXIMUM_SCAN_LINE_REG 0x09
#define VGA_CURSOR_START_REG 0x0A
#define VGA_CURSOR_END_REG 0x0B
#define VGA_START_ADDR_HIGH_REG 0x0C
#define VGA_START_ADDR_LOW_REG 0x0D
#define VGA_CURSOR_LOCATION_HIGH_REG 0x0E
#define VGA_CURSOR_LOCATION_LOW_REG 0x0F

#define VGA_ERASE_CHAR (vga_get_pair(' ', vga_get_attr(CONSOLE_COLOR_LIGHT_GREY, CONSOLE_COLOR_BLACK)))

static uint8_t g_console_buffer[NBR_AVAILABLE_CONSOLE][VGA_COLOR_FRAMEBUFFER_SIZE] = {0};
static size_t g_console_buffer_idx = 0;

static uint16_t *g_vga_vram_start = (uint16_t *)VGA_COLOR_FRAMEBUFFER_ADDR;
static uint16_t *g_vga_vram_end = (uint16_t *)(VGA_COLOR_FRAMEBUFFER_ADDR + VGA_COLOR_FRAMEBUFFER_SIZE);

static void
vga_clear_vram(uint16_t *start, uint16_t *end)
{
    for (uint16_t *ptr = start; ptr < end; ptr++)
    {
        *ptr = VGA_ERASE_CHAR;
    }
}

static void
vga_set_addr(const console_t *con)
{
    uint32_t offset;

    offset = (con->viewport_visible_origin - (uintptr_t)g_vga_vram_start) / 2;

    output_byte(VGA_CRTC_ADDR_REG, VGA_START_ADDR_LOW_REG);
    output_byte(VGA_CRTC_DATA_REG, offset & 0xFF);
    output_byte(VGA_CRTC_ADDR_REG, VGA_START_ADDR_HIGH_REG);
    output_byte(VGA_CRTC_DATA_REG, (offset >> 8) & 0xFF);
}

static void
vga_set_attr(console_t *c, console_color_t fg, console_color_t bg)
{
    c->attr = vga_get_attr(fg, bg);
}

static void
vga_set_cursor_style(const console_cursor_t cursor)
{
    uint8_t cursor_scanline_start = 0;
    uint8_t cursor_scanline_end = 0;
    uint8_t cursor_start = 0;
    uint8_t cursor_end = 0;

    switch (cursor)
    {
    case CONSOLE_CURSOR_OFF:
        cursor_scanline_start = 0;
        cursor_scanline_end = 0;
        break;
    case CONSOLE_CURSOR_BLOCK:
        output_byte(VGA_CRTC_ADDR_REG, VGA_MAXIMUM_SCAN_LINE_REG);
        cursor_scanline_start = 0;
        cursor_scanline_end = input_byte(VGA_CRTC_DATA_REG) & 0x0F;
        break;
    case CONSOLE_CURSOR_HALF_BLOCK:
        output_byte(VGA_CRTC_ADDR_REG, VGA_MAXIMUM_SCAN_LINE_REG);
        cursor_scanline_start = 0;
        cursor_scanline_end = (input_byte(VGA_CRTC_DATA_REG) & 0x0F) / 2;
        break;
    case CONSOLE_CURSOR_UNDERLINE:
        cursor_scanline_start = 0;
        cursor_scanline_end = 1;
        break;
    }

    cursor_start = (cursor == CONSOLE_CURSOR_OFF ? 0x20 : 0x00) | cursor_scanline_start;
    cursor_end = cursor_scanline_end;

    output_byte(VGA_CRTC_ADDR_REG, VGA_CURSOR_START_REG);
    output_byte(VGA_CRTC_DATA_REG, cursor_start);
    output_byte(VGA_CRTC_ADDR_REG, VGA_CURSOR_END_REG);
    output_byte(VGA_CRTC_DATA_REG, cursor_end);
}

/**
 * @brief A 'softscroll' is a scrolling mechanism that does not modify the VGA framebuffer. It moves the visible origin
 * of the console and update the VGA Start Address register.
 *
 * @param con Console handle
 * @param dir Direction of the softscroll
 * @param n How many lines to scroll.
 * @return true Can scroll.
 * @return false Cannot scroll.
 */
static bool
vga_softscroll(console_t *con, console_scroll_dir_t dir, size_t n)
{
    size_t delta = n * con->viewport_row_size;

    if (dir == CONSOLE_SCROLL_UP)
    {
        if (con->viewport_visible_origin - delta < (uintptr_t)g_vga_vram_start)
        {
            delta = con->viewport_visible_origin - (uintptr_t)g_vga_vram_start;
        }
        con->viewport_visible_origin -= delta;
        con->viewport_visible_end -= delta;
    }
    else if (dir == CONSOLE_SCROLL_DOWN)
    {
        if (con->viewport_visible_end + delta > con->viewport_end)
        {
            delta = con->viewport_end - con->viewport_visible_end;
        }
        con->viewport_visible_origin += delta;
        con->viewport_visible_end += delta;
    }
    if (delta) {
        vga_set_addr(con);
    }
    return delta != 0;
}

/**
 * @brief Restore
 *
 * @param con
 */
void vga_restore(console_t *con)
{
    con->viewport_visible_origin = con->viewport_origin;
    con->viewport_visible_end = con->viewport_end;

    vga_set_addr(con);
}

static bool
vga_put_char(console_t *con, unsigned char c, size_t x, size_t y)
{
    uint16_t *ptr;

    if (x >= con->viewport_col_nbr || y >= con->viewport_row_nbr)
    {
        return false;
    }

    ptr = (uint16_t *)(con->viewport_origin + (y * con->viewport_row_size + x * con->viewport_col_size));
    *ptr = vga_get_pair(c, con->attr);

    return true;
}

static bool
vga_scroll(console_t *con, console_scroll_dir_t dir)
{
    size_t delta;

    /* Scroll one line down or up. */
    delta = con->viewport_row_size;
    switch (dir)
    {
    case CONSOLE_SCROLL_UP:
        if (con->viewport_end + delta > (uintptr_t)g_vga_vram_end)
        {
            /* If the visible viewport would exceed the VRAM size, wrap around to the beginning of the VRAM. */
            memcpy(g_vga_vram_start, (const void *)(con->viewport_origin + delta), con->viewport_size - delta);
            con->viewport_origin = (uintptr_t)g_vga_vram_start;
            con->viewport_end = con->viewport_origin + con->viewport_size;
        }
        else
        {
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
    con->viewport_visible_end = con->viewport_end;

    vga_set_addr(con);
    return true;
}

static bool
vga_set_cursor_pos(const console_t *con, const size_t x, const size_t y)
{
    if (x >= con->viewport_col_nbr || y >= con->viewport_row_nbr)
    {
        return false;
    }

    const uintptr_t delta = (con->viewport_origin - (uintptr_t)g_vga_vram_start) / 2;
    const uint16_t pos = delta + y * con->viewport_col_nbr + x;

    output_byte(VGA_CRTC_ADDR_REG, VGA_CURSOR_LOCATION_LOW_REG);
    output_byte(VGA_CRTC_DATA_REG, pos & 0xFF);

    output_byte(VGA_CRTC_ADDR_REG, VGA_CURSOR_LOCATION_HIGH_REG);
    output_byte(VGA_CRTC_DATA_REG, (pos >> 8) & 0xFF);

    return true;
}

static void
vga_load(const console_t *con)
{
    memcpy(g_vga_vram_start, (const void *)con->buffer, con->buffer_size);
    vga_set_addr(con);
}

static void
vga_save(console_t *con)
{
    memcpy((void *)con->buffer, g_vga_vram_start, con->buffer_size);
}

static void
vga_init(console_t *con)
{
    if (g_console_buffer_idx >= NBR_AVAILABLE_CONSOLE)
    {
        return;
    }

    con->buffer = (uintptr_t)g_console_buffer[g_console_buffer_idx];
    con->buffer_size = VGA_COLOR_FRAMEBUFFER_SIZE;
    g_console_buffer_idx++;

    vga_clear_vram((uint16_t *)con->buffer, (uint16_t *)(con->buffer + con->buffer_size));

    con->viewport_col_size = 2;
    con->viewport_col_nbr = VGA_FRAMEBUFFER_WIDTH;

    con->viewport_row_nbr = VGA_FRAMEBUFFER_HEIGHT;
    con->viewport_row_size = con->viewport_col_nbr * con->viewport_col_size;

    con->viewport_size = con->viewport_row_nbr * con->viewport_row_size;

    con->viewport_origin = (uintptr_t)g_vga_vram_start;
    con->viewport_visible_origin = con->viewport_origin;

    con->viewport_end = con->viewport_origin + con->viewport_size;
    con->viewport_visible_end = con->viewport_end;

    con->attr = vga_get_attr(CONSOLE_COLOR_LIGHT_GREY, CONSOLE_COLOR_BLACK);
}

console_impl_t VGA_CONSOLE_DRIVER = {
    .init = vga_init,
    .put_char = vga_put_char,
    .softscroll = vga_softscroll,
    .scroll = vga_scroll,
    .restore = vga_restore,
    .load = vga_load,
    .save = vga_save,
    .set_cursor_pos = vga_set_cursor_pos,
    .set_cursor_style = vga_set_cursor_style,
    .set_attr = vga_set_attr,
    .max_console_nbr = NBR_AVAILABLE_CONSOLE,
};