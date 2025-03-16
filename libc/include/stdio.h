#ifndef STDIO_H
#define STDIO_H

#ifdef __cplusplus
extern "C"
{
#endif

    int printf(const char *format, ...);
    int putchar(int c);
    int puts(const char *s);

#ifdef __cplusplus
}
#endif

#endif