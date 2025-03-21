#include <kernel/io/helper.h>
#include <kernel/multiboot.h>
#include <kernel/io/keyboard/ps2_keyboard.h>
#include <kernel/io/keyboard/layout_qwerty.h>
#include <kernel/io/vga_terminal.h>

#include <stdio.h>

__attribute__((constructor)) static void kernel_init()
{
    vga_terminal_init();
    vga_terminal_set_attribute(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    kbd_init(&kbd_qwerty_layout);

    puts("Kernel is starting...\n");
}

__attribute__((destructor)) static void kernel_fini()
{
    vga_terminal_set_attribute(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    puts("Kernel is shutting down...");
}

void kernel_main(uint32_t magic, multiboot_info_t *mbi)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        return;
    }

    (void)mbi;

    const kbd_event_t *kbd_event = NULL;

    while (1)
    {
        while (!(input_byte(0x64) & 0x01))
        {
        }

        kbd_event = kbd_on_input(input_byte(0x60));
        if (kbd_event != NULL)
        {
            unsigned char ascii = kbd_translate_event(kbd_event);
            if (ascii != 0)
            {
                vga_terminal_write((const char *)&ascii, 1);
            }
        }
    }

    return;
}