#ifndef IO_HPP
#define IO_HPP

#include <stdint.h>

namespace IO {
    inline uint8_t inputByte(uint32_t port)
    {
        uint8_t ret;
        asm volatile("inb %%dx,%%al" : "=a"(ret) : "d"(port));
        return ret;
    }

    inline void ouputByte(uint32_t port, uint8_t value)
    {
        asm volatile("outb %%al,%%dx" : : "d"(port), "a"(value));
    }
}

#endif