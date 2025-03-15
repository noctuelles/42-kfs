#include <string.h>
#include <kernel/io/VGATerminal.hpp>
#include <kernel/Multiboot.hpp>


extern "C" void kernelMain(uint32_t magic, multiboot_info_t *multiboot_info) {
    IO::VGATerminal vgaTerminal{};

    if (magic != MULTIBOOT_HEADER_MAGIC) {
        return;
    }

    (void)multiboot_info;

    return;
}