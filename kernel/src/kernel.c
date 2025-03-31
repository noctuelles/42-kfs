#include <kernel/io/console/vga.h>
#include <kernel/io/helper.h>
#include <kernel/io/keyboard/layout_qwerty.h>
#include <kernel/io/keyboard/ps2_keyboard.h>
#include <kernel/io/terminal.h>
#include <kernel/multiboot.h>
#include <stdio.h>

__attribute__((constructor)) static void
kernel_init() {
    terminal_init(&VGA_CONSOLE_DRIVER);
    kbd_init(&kbd_qwerty_layout);
}

__attribute__((destructor)) static void
kernel_fini() {}

void
kernel_main(uint32_t magic, multiboot_info_t *mbi) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        return;
    }

    (void)mbi;

    const kbd_event_t *kbd_event = NULL;

    while (1) {
        while (!(input_byte(0x64) & 0x01)) {
        }

        kbd_event = kbd_on_input(input_byte(0x60));
        if (kbd_event != NULL) {
            unsigned char ascii = kbd_translate_event(kbd_event);
            if (ascii != 0) {
                terminal_write(&ascii, 1);
            }
        }
    }
}