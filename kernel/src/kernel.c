#include <kernel/io/vga_terminal.h>
#include <kernel/multiboot.h>

#include <stdio.h>

__attribute__((constructor)) static void kernel_init()
{
    vga_terminal_init();
    vga_terminal_set_attribute(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    puts("Kernel is starting...\n");
}

__attribute__((destructor)) static void kernel_fini()
{
    vga_terminal_set_attribute(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    puts("Kernel is shutting down...");
}

void kernel_main(uint32_t magic, multiboot_info_t *multiboot_info)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        return;
    }

    printf("Hello World, the multiboot header is at %#08x !\n", multiboot_info);
    vga_terminal_set_attribute(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    puts("I can print in color too !");
    vga_terminal_set_attribute(VGA_COLOR_BLUE, VGA_COLOR_RED);
    puts("Also with ugly colors...");

    return;
}