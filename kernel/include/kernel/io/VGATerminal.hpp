#ifndef VGATERMINAL_HPP
#define VGATERMINAL_HPP

#include <stdint.h>
#include <stddef.h>
namespace IO
{
    class VGATerminal
    {
    public:
        static constexpr size_t WIDTH{80};
        static constexpr size_t HEIGHT{25};

        enum Color : uint8_t
        {
            BLACK = 0,
            BLUE = 1,
            GREEN = 2,
            CYAN = 3,
            RED = 4,
            MAGENTA = 5,
            BROWN = 6,
            LIGHT_GREY = 7,
            DARK_GREY = 8,
            LIGHT_BLUE = 9,
            LIGHT_GREEN = 10,
            LIGHT_CYAN = 11,
            LIGHT_RED = 12,
            LIGHT_MAGENTA = 13,
            LIGHT_BROWN = 14,
            WHITE = 15,
        };

        VGATerminal();

        void putString(const char *str, Color fg = Color::LIGHT_GREY, Color bg = Color::BLACK);
        void putChar(unsigned char c, Color fg = Color::LIGHT_GREY, Color bg = Color::BLACK);
        void putFormat(const char *format, ...);

    private:
        size_t row{0};
        size_t column{0};
        uint16_t *buffer{reinterpret_cast<uint16_t *>(0xB8000)};
    };

}

#endif