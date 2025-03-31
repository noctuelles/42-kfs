#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef enum flags_e {
    ALTERNATE_FORM = 1 << 0,
    ZERO_PADDED    = 1 << 1,
    LEFT_JUSTIFIED = 1 << 2,
} flags_t;

typedef enum length_modifier_e {
    NONE,
    HH,
    H,
    L,
    LL,
    J,
    Z,
    T,
    L_CAPITAL,
} length_modifier_t;

typedef struct format_info_s {
    flags_t           flags;
    int               width;
    length_modifier_t length_modifier;
} format_info_t;

static int
write_string(const char *s) {
    size_t i = 0;
    while (s[i] != '\0') {
        putchar(s[i]);
        i++;
    }
    return i;
}

static flags_t
parse_flags(const char **format) {
    flags_t flags = 0;

    while (1) {
        switch (**format) {
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

static int
parse_width(const char **format) {
    int width = 0;

    while (**format >= '0' && **format <= '9') {
        width = width * 10 + (**format - '0');
        (*format)++;
    }

    return width;
}

static int
parse_length_modifier(const char **format) {
    switch (**format) {
        case 'h':
            (*format)++;
            if (**format == 'h') {
                (*format)++;
                return HH;
            }
            return H;
        case 'l':
            (*format)++;
            if (**format == 'l') {
                (*format)++;
                return LL;
            }
            return L;
        case 'j':
            (*format)++;
            return J;
        case 'z':
            (*format)++;
            return Z;
        case 't':
            (*format)++;
            return T;
        case 'L':
            (*format)++;
            return L_CAPITAL;
        default:
            return NONE;
    }
}

static int
print_hex(format_info_t *format_info, uint64_t value, bool upper) {
    const char *digits    = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    const char *alternate = upper ? "0X" : "0x";
    const char *str       = NULL;
    int         ret       = 0;

    str = utoa_base(value, digits);
    if (str == NULL) {
        return -1;
    }

    if (format_info->flags & LEFT_JUSTIFIED) {
        if (format_info->flags & ALTERNATE_FORM) {
            ret += write_string(alternate);
        }
        ret += write_string(str);
        for (int out_size = (size_t)ret + strlen(str); out_size < format_info->width; out_size++) {
            putchar(' ');
            ret++;
        }
    } else {
        if (format_info->flags & ZERO_PADDED) {
            if (format_info->flags & ALTERNATE_FORM) {
                ret += write_string(alternate);
            }

            for (int out_size = (size_t)ret + strlen(str); out_size < format_info->width; out_size++) {
                putchar('0');
                ret++;
            }

            ret += write_string(str);
        } else {
            if (format_info->flags & ALTERNATE_FORM) {
                ret += 2;
            }

            for (int out_size = (size_t)ret + strlen(str); out_size < format_info->width; out_size++) {
                putchar(' ');
                ret++;
            }

            if (format_info->flags & ALTERNATE_FORM) {
                write_string(alternate);
            }

            ret += write_string(str);
        }
    }

    return ret;
}

int
printf(const char *format, ...) {
    int           ret        = 0;
    int           print_ret  = 0;
    va_list       parameters = {0};
    format_info_t format_info;

    va_start(parameters, format);
    while (*format != '\0') {
        if (*format == '%') {
            memset(&format_info, 0, sizeof(format_info_t));
            format++;
            format_info.flags           = parse_flags(&format);
            format_info.width           = parse_width(&format);
            format_info.length_modifier = parse_length_modifier(&format);

            switch (*format) {
                case 'c':
                case '%':
                    /*TODO: Flags etc.*/
                    putchar(va_arg(parameters, int));
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
                case 'u':
                    print_ret = write_string(utoa_base(va_arg(parameters, uint32_t), "0123456789"));
                    break;
                case 'x':
                    if (format_info.length_modifier == L) {
                        print_ret = print_hex(&format_info, va_arg(parameters, uint64_t), false);
                    } else {
                        print_ret = print_hex(&format_info, va_arg(parameters, uint32_t), false);
                    }
                    break;
                case 'X':
                    if (format_info.length_modifier == L) {
                        print_ret = print_hex(&format_info, va_arg(parameters, uint64_t), true);
                    } else {
                        print_ret = print_hex(&format_info, va_arg(parameters, uint32_t), true);
                    }
                    break;
            }

            if (print_ret < 0) {
                return -1;
            }

            ret += print_ret;
        } else {
            putchar(*format);
            ret++;
        }

        format++;
    }

    va_end(parameters);
    return ret;
}