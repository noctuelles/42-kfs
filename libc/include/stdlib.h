#ifndef STDLIB_H
#define STDLIB_H

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C"
{
#endif

    __attribute__((__noreturn__)) void abort(void);
    int atoi(const char *nptr);

#ifdef __cplusplus
}
#endif

#endif