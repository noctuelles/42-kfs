#include <kernel/io/console.h>

static console_impl_t *g_console_impl;
static console_t g_console;

static size_t x;
static size_t y;

static void terminal_putc(unsigned char c) {
    switch (c) {
        case '\n':
            x = 0;
            y += 1;
            break;
        default:
            g_console_impl->put_char(&g_console, c, x, y);
            x += 1;
            break;
    }

    if (x == g_console.viewport_col_nbr) {
        x = 0;
        y += 1;
    }
}

void terminal_init(console_impl_t *console_impl) {
    x = 0;
    y = 0;

    console_impl->init(&g_console);
    console_impl->set_cursor_style(CONSOLE_CURSOR_OFF);
    console_impl->set_cursor_pos(&g_console, x, y);
    g_console_impl = console_impl;
}

void terminal_write(const unsigned char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putc(buffer[i]);
    }
}