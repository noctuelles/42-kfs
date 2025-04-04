#ifndef IO_H
#define IO_H

#include <stdint.h>

inline uint8_t input_byte(uint16_t port)
{
    uint8_t ret;
    asm volatile("in al, dx" : "=a"(ret) : "d"(port));
    return ret;
}

inline void output_byte(uint16_t port, uint8_t value)
{
    asm volatile("out dx, al" : : "d"(port), "a"(value));
}

#endif