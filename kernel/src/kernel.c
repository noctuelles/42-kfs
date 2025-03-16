#include <kernel/io/vga_terminal.h>
#include <kernel/multiboot.h>

#include <stdio.h>

__attribute__((constructor)) static void kernel_init()
{
    vga_terminal_init();
    vga_terminal_set_attribute(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

void kernel_main(uint32_t magic, multiboot_info_t *multiboot_info)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        return;
    }
    (void)multiboot_info;

    puts("Hello 42!\n");

    return;
}