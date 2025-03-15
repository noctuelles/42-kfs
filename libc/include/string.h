#ifndef STRING_H
#define STRING_H

#include <sys/cdefs.h>
#include <stdint.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int memcmp(const void *, const void *, size_t);
    void *memcpy(void *__restrict, const void *__restrict, size_t);
    void *memmove(void *, const void *, size_t);
    void *memset(void *, int, size_t);
    size_t strlen(const char *);
    char *itoa_base(int32_t n, const char *base);

#ifdef __cplusplus
}
#endif

#endif