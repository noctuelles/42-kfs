#include <kernel/io/console.h>

static console_impl_t *g_ci;
static console_t g_console;

static size_t x;
static size_t y;

static void terminal_putc(unsigned char c) {
    switch (c) {
        case '\n':
            x = 0;
            y += 1;
            break;
        case '\b':
            if (x > 0) {
                x -= 1;
            }
            g_ci->put_char(&g_console, ' ', x, y);
            break;
        default:
            g_ci->put_char(&g_console, c, x, y);
            x += 1;
            break;
    }
    if (x >= g_console.viewport_col_nbr) {
        x = 0;
        y += 1;
    }
    if (y >= g_console.viewport_row_nbr) {
        g_ci->scroll(&g_console, CONSOLE_SCROLL_UP);
        x = 0;
        y -= 1;
    }

    g_ci->set_cursor_pos(&g_console, x, y);
}

void terminal_init(console_impl_t *ci) {
    x = 0;
    y = 0;

    ci->init(&g_console);
    ci->set_cursor_style(CONSOLE_CURSOR_BLOCK);
    ci->set_cursor_pos(&g_console, x, y);
    ci->_switch(&g_console);

    g_ci = ci;
}

void terminal_write(const char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putc((unsigned char)buffer[i]);
    }
}