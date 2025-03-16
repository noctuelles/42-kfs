#ifndef CTYPE_H
#define CTYPE_H

#ifdef __cplusplus
extern "C"
{
#endif

    inline int isdigit(int c)
    {
        return c >= '0' && c <= '9';
    }

    inline int isspace(int c)
    {
        return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
    }

    inline int isalpha(int c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    inline int isalnum(int c)
    {
        return isalpha(c) || isdigit(c);
    }

    inline int isprint(int c)
    {
        return c >= 0x20 && c <= 0x7E;
    }

    inline int islower(int c)
    {
        return c >= 'a' && c <= 'z';
    }

    inline int isupper(int c)
    {
        return c >= 'A' && c <= 'Z';
    }

#ifdef __cplusplus
}
#endif

#endif