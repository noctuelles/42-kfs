#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <kernel/io/console.h>

void terminal_init(console_impl_t *console_impl);
void terminal_write(const char *buffer, size_t size);
void terminal_scroll(console_scroll_dir_t dir, size_t n);
void terminal_set_color(console_color_t foreground, console_color_t background);
void terminal_switch();

#endif