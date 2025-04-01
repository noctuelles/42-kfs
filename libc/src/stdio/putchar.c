#ifdef __is_libk
#include <kernel/io/terminal.h>
#endif

int putchar(int c)
{
#ifdef __is_libk
    terminal_write((const char *)&c, sizeof(unsigned char));
#else
/* TODO */
#endif
    return (unsigned char)c;
}