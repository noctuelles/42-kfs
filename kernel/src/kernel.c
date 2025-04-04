#include <kernel/io/console/vga.h>
#include <kernel/io/helper.h>
#include <kernel/io/keyboard/layout_qwerty.h>
#include <kernel/io/keyboard/ps2_keyboard.h>
#include <kernel/io/terminal.h>
#include <kernel/multiboot.h>
#include <stdio.h>
#include <string.h>

__attribute__((constructor)) static void
kernel_init() {
    terminal_init(&VGA_CONSOLE_DRIVER);
    kbd_init(&KBD_QWERTY_LAYOUT);
}

__attribute__((destructor)) static void
kernel_fini() {}

static const char *
readline(const char *prefix) {
    static char        buffer[56] = {0};
    const kbd_event_t *kbd_event  = NULL;
    unsigned char      ascii      = 0;
    size_t             i          = 0;


    terminal_set_color(CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_BLACK);
    printf(prefix);
    terminal_set_color(CONSOLE_COLOR_LIGHT_GREY, CONSOLE_COLOR_BLACK);
    printf("> ");

    while (1) {
        while (!(input_byte(0x64) & 0x01)) {
            /* Wait for input... */
        }

        kbd_event = kbd_on_input(input_byte(0x60));
        if (kbd_event != NULL) {
            kbd_event_t crtl = kbd_get_key_state(VK_LCONTROL);

            if (!kbd_event->flags.is_pressed) {
                /* Ignore key releases. */
                continue;
            }

            if (crtl.flags.is_pressed) {
                if (kbd_event->vk == VK_U) {
                    terminal_scroll(CONSOLE_SCROLL_UP, 5);
                } else if (kbd_event->vk == VK_D) {
                    terminal_scroll(CONSOLE_SCROLL_DOWN, 5);
                }
                continue;
            }

            ascii = kbd_translate_event(kbd_event);

            if (!ascii) {
                continue;
            }

            if (ascii == '\n') {
                buffer[i] = '\0';
                putchar('\n');
                return buffer;
            }

            if (ascii == '\b') {
                if (i > 0) {
                    i--;
                    putchar('\b');
                }
                continue;
            }

            if (i < sizeof(buffer) - 1) {
                buffer[i++] = ascii;
                putchar(ascii);
            }
        }
    }

    return NULL;
}

void
kernel_main(uint32_t magic, multiboot_info_t *mbi) {
    const char *command = NULL;
    (void)mbi;

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        return;
    }

    puts("Type 'help' for a list of commands.\n");

    while (1) {
        command = readline("kfs");

        if (command && *command) {

            if (strcmp(command, "switch") == 0) {
                terminal_switch();
            } else if (strcmp(command, "help") == 0) {
                puts("switch - switch to another virtual terminal.");
            } else {
                printf("command not found: %s\n", command);
            }
        }
    }
}