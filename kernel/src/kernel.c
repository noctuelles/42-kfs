#include <kernel/io/console/vga.h>
#include <kernel/io/helper.h>
#include <kernel/io/terminal.h>
#include <kernel/multiboot.h>
#include <stdio.h>

__attribute__((constructor)) static void
kernel_init() {
    terminal_init(&VGA_CONSOLE_DRIVER);
}

__attribute__((destructor)) static void
kernel_fini() {}

void
kernel_main(uint32_t magic, multiboot_info_t *mbi) {
    char letter = 'a';
    bool debounce = false;

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        return;
    }

    while (1) {
        while (!(input_byte(0x64) & 0x01)) {
        }
        (void)input_byte(0x60);
        if (debounce) {
            debounce = false;
            continue;
        }
        printf("%c\n", letter);
        letter += 1;
        debounce = true;
    }

    (void)mbi;
}