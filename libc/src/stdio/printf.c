#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum flags_e
{
    ALTERNATE_FORM = 1 << 0,
    ZERO_PADDED = 1 << 1,
    LEFT_JUSTIFIED = 1 << 2,
} flags_t;

typedef struct format_info_s
{
    flags_t flags;
    int width;
} format_info_t;

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

static flags_t parse_flags(const char **format)
{
    flags_t flags = 0;

    while (1)
    {
        switch (**format)
        {
        case '#':
            flags |= ALTERNATE_FORM;
            break;
        case '0':
            flags |= ZERO_PADDED;
            break;
        case '-':
            flags |= LEFT_JUSTIFIED;
            break;
        default:
            return flags;
        }
        (*format)++;
    }
    __builtin_unreachable();
}

static int parse_width(const char **format)
{
    int width = 0;

    while (**format >= '0' && **format <= '9')
    {
        width = width * 10 + (**format - '0');
        (*format)++;
    }

    return width;
}

static int print_hex(format_info_t *format_info, uint32_t value, bool upper)
{
    const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    const char *alternate = upper ? "0X" : "0x";
    const char *str = NULL;
    int ret = 0;

    str = itoa_base(value, digits);
    if (str == NULL)
    {
        return -1;
    }

    if (format_info->flags & LEFT_JUSTIFIED)
    {
        if (format_info->flags & ALTERNATE_FORM)
        {
            ret += write_string(alternate);
        }
        ret += write_string(str);
        for (int out_size = (size_t)ret + strlen(str); out_size < format_info->width; out_size++)
        {
            putchar(' ');
            ret++;
        }
    }
    else
    {
        if (format_info->flags & ZERO_PADDED)
        {
            if (format_info->flags & ALTERNATE_FORM)
            {
                ret += write_string(alternate);
            }

            for (int out_size = (size_t)ret + strlen(str); out_size < format_info->width; out_size++)
            {
                putchar('0');
                ret++;
            }

            ret += write_string(str);
        }
        else
        {
            if (format_info->flags & ALTERNATE_FORM)
            {
                ret += 2;
            }

            for (int out_size = (size_t)ret + strlen(str); out_size < format_info->width; out_size++)
            {
                putchar(' ');
                ret++;
            }

            if (format_info->flags & ALTERNATE_FORM)
            {
                write_string(alternate);
            }

            ret += write_string(str);
        }
    }

    return ret;
}

int printf(const char *format, ...)
{
    int ret = 0;
    int print_ret = 0;
    va_list parameters = {0};
    format_info_t format_info;

    va_start(parameters, format);
    while (*format != '\0')
    {
        if (*format == '%')
        {
            memset(&format_info, 0, sizeof(format_info_t));
            format++;
            format_info.flags = parse_flags(&format);
            format_info.width = parse_width(&format);

            switch (*format)
            {
            case 'c':
            case '%':
                /*TODO: Flags etc.*/
                putchar(*format);
                print_ret = 1;
                break;
            case 's':
                /*TODO: Flags etc.*/
                print_ret = write_string(va_arg(parameters, const char *));
                break;
            case 'd':
            case 'i':
                /*TODO: Flags etc.*/
                print_ret = write_string(itoa_base(va_arg(parameters, int32_t), "0123456789"));
                break;
            case 'x':
                print_ret = print_hex(&format_info, va_arg(parameters, uint32_t), false);
                break;
            case 'X':
                print_ret = print_hex(&format_info, va_arg(parameters, uint32_t), true);
                break;
            }

            if (print_ret < 0)
            {
                return -1;
            }

            ret += print_ret;
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