#include <kernel/io/VGATerminal.hpp>
#include <stdarg.h>
#include <string.h>

using namespace IO;

VGATerminal::VGATerminal()
{
    /* Clear screen buffer. */
    for (size_t y = 0; y < HEIGHT; y++)
    {
        for (size_t x = 0; x < WIDTH; x++)
        {
            buffer[y * WIDTH + x] = 0x0020;
        }
    }
}

void VGATerminal::putString(const char *str, Color fg, Color bg)
{
    while (*str)
    {
        putChar(*str, fg, bg);
        str++;
    }
}

void VGATerminal::putChar(unsigned char c, Color fg, Color bg)
{
    const auto attribute{static_cast<uint8_t>(fg | bg << 4)};

    if (c == '\n')
    {
        column = 0;
        row++;
        return;
    }

    buffer[row * WIDTH + column] = c | attribute << 8;

    if (++column == WIDTH)
    {
        column = 0;
        row++;
    }
}

void VGATerminal::putFormat(const char *format, ...)
{
    va_list parameters;
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
                putChar(*format);
                break;
            case 's':
                putString(va_arg(parameters, const char *));
                break;
            case 'd':
            case 'i':
                putString(itoa_base(va_arg(parameters, int32_t), "0123456789"));
                break;
            case 'x':
                putString(itoa_base(va_arg(parameters, uint32_t), "0123456789abcdef"));
                break;
            case 'X':
                putString(itoa_base(va_arg(parameters, uint32_t), "0123456789ABCDEF"));
                break;
            }
        }
        else
        {
            putChar(*format);
        }

        format++;
    }

    va_end(parameters);
}