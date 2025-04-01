#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <kernel/io/console.h>

void terminal_init(console_impl_t *console_impl);
void terminal_write(const char *buffer, size_t size);
void terminal_switch();

#endif