#include <stdio.h>
#include <stddef.h>

int puts(const char *s)
{
    size_t i = 0;
    while (s[i] != '\0') {
        putchar(s[i]);
        i++;
    }
    putchar('\n');
    return i + 1;
}