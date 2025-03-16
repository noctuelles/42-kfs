#include <kernel/io/vga_terminal.h>
#include <stddef.h>

static size_t row = 0;
static size_t column = 0;
static uint16_t *buffer = (uint16_t *)0xB8000;
static uint8_t fg = VGA_COLOR_LIGHT_GREY;
static uint8_t bg = VGA_COLOR_BLACK;

void vga_terminal_init(void)
{
    /* Clear screen buffer. */
    for (size_t y = 0; y < VGA_TERMINAL_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_TERMINAL_WIDTH; x++)
        {
            buffer[y * VGA_TERMINAL_WIDTH + x] = 0x0020;
        }
    }
}

void vga_terminal_put_char(unsigned char c)
{
    const uint8_t attribute = fg | bg << 4;

    if (c == '\n')
    {
        column = 0;
        row++;
        return;
    }
    buffer[row * VGA_TERMINAL_WIDTH + column] = c | attribute << 8;
    column++;
    if (column == VGA_TERMINAL_WIDTH)
    {
        column = 0;
        row++;
        if (row == VGA_TERMINAL_HEIGHT)
        {
            /* TODO: scroll. */
        }
    }
}

void vga_terminal_write(const char *buffer, size_t len)
{
    if (buffer == NULL)
    {
        return;
    }

    for (size_t i = 0; i < len; i++)
    {
        vga_terminal_put_char((unsigned char)buffer[i]);
    }
}

void vga_terminal_set_attribute(vga_color_t fg, vga_color_t bg)
{
    fg = fg;
    bg = bg;
}