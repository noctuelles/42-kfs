#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

static int write_string(const char *s)
{
    size_t i = 0;
    while (s[i] != '\0')
    {
        putchar(s[i]);
        i++;
    }
    return i;
}

int printf(const char *format, ...)
{
    int ret = 0;
    va_list parameters = {0};

    va_start(parameters, format);
    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;

            switch (*format)
            {
            case 'c':
            case '%':
                putchar(*format);
                ret++;
                break;
            case 's':
                ret += write_string(va_arg(parameters, const char *));
                break;
            case 'd':
            case 'i':
                ret += write_string(itoa_base(va_arg(parameters, int32_t), "0123456789"));
                break;
            case 'x':
                ret += write_string(itoa_base(va_arg(parameters, uint32_t), "0123456789abcdef"));
                break;
            case 'X':
                ret += write_string(itoa_base(va_arg(parameters, uint32_t), "0123456789ABCDEF"));
                break;
            }
        }
        else
        {
            putchar(*format);
            ret++;
        }

        format++;
    }

    va_end(parameters);
    return ret;
}