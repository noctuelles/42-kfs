#include <kernel/io/console.h>
typedef struct terminal_s {
    size_t    x;
    size_t    y;
    console_t console;
} terminal_t;

static console_impl_t *con_impl;

static terminal_t  terms[NBR_AVAILABLE_CONSOLE];
static size_t      term_idx    = 0;
static terminal_t *active_term = NULL;

static void
terminal_putc(unsigned char c) {
    switch (c) {
        case '\n':
            active_term->x = 0;
            active_term->y += 1;
            break;
        case '\b':
            if (active_term->x > 0) {
                active_term->x -= 1;
            }
            con_impl->put_char(&active_term->console, ' ', active_term->x, active_term->y);
            break;
        default:
            con_impl->put_char(&active_term->console, c, active_term->x, active_term->y);
            active_term->x += 1;
            break;
    }

    if (active_term->x >= active_term->console.viewport_col_nbr) {
        active_term->x = 0;
        active_term->y += 1;
    }
    if (active_term->y >= active_term->console.viewport_row_nbr) {
        con_impl->scroll(&active_term->console, CONSOLE_SCROLL_UP);
        active_term->x = 0;
        active_term->y -= 1;
    }

    con_impl->set_cursor_pos(&active_term->console, active_term->x, active_term->y);
}

void
terminal_switch() {
    con_impl->save(&active_term->console);

    if (term_idx == 0) {
        term_idx = 1;
    } else {
        term_idx = 0;
    }

    active_term = &terms[term_idx];

    con_impl->load(&active_term->console);
    con_impl->set_cursor_pos(&active_term->console, active_term->x, active_term->y);
}


void terminal_scroll(console_scroll_dir_t dir, size_t n) {
    con_impl->softscroll(&active_term->console, dir, n);
}


void terminal_set_color(console_color_t foreground, console_color_t background) {
    con_impl->set_attr(&active_term->console, foreground, background);
}

void
terminal_init(console_impl_t *ci) {
    con_impl = ci;

    for (size_t i = 0; i < NBR_AVAILABLE_CONSOLE; i++) {
        terms[i].x = 0;
        terms[i].y = 0;
        con_impl->init(&terms[i].console);
    }

    term_idx = 0;
    active_term = &terms[term_idx];
    con_impl->load(&active_term->console);
}

void
terminal_write(const char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putc((unsigned char)buffer[i]);
    }
}