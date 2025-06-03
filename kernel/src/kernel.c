#include <kernel/io/console/vga.h>
#include <kernel/io/helper.h>
#include <kernel/io/keyboard.h>
#include <kernel/io/keyboard/layout_qwerty.h>
#include <kernel/io/keyboard/ps2_keyboard.h>
#include <kernel/io/terminal.h>
#include <kernel/multiboot.h>
#include <stdio.h>
#include <string.h>

__attribute__((constructor)) static void
kernel_init()
{
    terminal_init(&VGA_CONSOLE_DRIVER);
    kbd_init(&KBD_QWERTY_LAYOUT);
}

__attribute__((destructor)) static void
kernel_fini() {}

static void print_multiboot(multiboot_info_t *mbi)
{
    printf("flags = 0x%x\n", (unsigned)mbi->flags);

    if (CHECK_FLAG(mbi->flags, 0))
        printf("mem_lower = %uKB, mem_upper = %uKB\n",
               (unsigned)mbi->mem_lower, (unsigned)mbi->mem_upper);

    if (CHECK_FLAG(mbi->flags, 1))
        printf("boot_device = 0x%x\n", (unsigned)mbi->boot_device);

    if (CHECK_FLAG(mbi->flags, 2))
        printf("cmdline = %s\n", (char *)mbi->cmdline);

    if (CHECK_FLAG(mbi->flags, 3))
    {
        multiboot_module_t *mod;
        uint32_t i;

        printf("mods_count = %d, mods_addr = 0x%x\n",
               (int)mbi->mods_count, (int)mbi->mods_addr);
        for (i = 0, mod = (multiboot_module_t *)mbi->mods_addr;
             i < mbi->mods_count;
             i++, mod++)
            printf(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                   (unsigned)mod->mod_start,
                   (unsigned)mod->mod_end,
                   (char *)mod->cmdline);
    }

    if (CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5))
    {
        printf("Both bits 4 and 5 are set.\n");
        return;
    }

    if (CHECK_FLAG(mbi->flags, 4))
    {
        multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

        printf("multiboot_aout_symbol_table: tabsize = 0x%0x, "
               "strsize = 0x%x, addr = 0x%x\n",
               (unsigned)multiboot_aout_sym->tabsize,
               (unsigned)multiboot_aout_sym->strsize,
               (unsigned)multiboot_aout_sym->addr);
    }

    if (CHECK_FLAG(mbi->flags, 5))
    {
        multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

        printf("multiboot_elf_sec: num = %u, size = 0x%x,"
               " addr = 0x%x, shndx = 0x%x\n",
               (unsigned)multiboot_elf_sec->num, (unsigned)multiboot_elf_sec->size,
               (unsigned)multiboot_elf_sec->addr, (unsigned)multiboot_elf_sec->shndx);
    }

    if (CHECK_FLAG(mbi->flags, 6))
    {
        multiboot_memory_map_t *mmap;

        printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
               (unsigned)mbi->mmap_addr, (unsigned)mbi->mmap_length);
        for (mmap = (multiboot_memory_map_t *)mbi->mmap_addr;
             (unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length;
             mmap = (multiboot_memory_map_t *)((unsigned long)mmap + mmap->size + sizeof(mmap->size)))
            printf(" size = 0x%x, base_addr = 0x%x%08x,"
                   " length = 0x%x%08x, type = 0x%x\n",
                   (unsigned)mmap->size,
                   (unsigned)(mmap->addr >> 32),
                   (unsigned)(mmap->addr & 0xffffffff),
                   (unsigned)(mmap->len >> 32),
                   (unsigned)(mmap->len & 0xffffffff),
                   (unsigned)mmap->type);
    }
    if (CHECK_FLAG(mbi->flags, 12))
    {
        printf("framebuffer_addr = 0x%X, ",
               (unsigned)mbi->framebuffer_addr);
        printf("framebuffer type = 0x%X\n",
               (unsigned)mbi->framebuffer_type);

        if (mbi->framebuffer_type == 0x2)
        {
            printf("framebuffer: width = %u, height = %u, "
                   "bpp = %u, pitch = %u\n",
                   (unsigned)mbi->framebuffer_width,
                   (unsigned)mbi->framebuffer_height,
                   (unsigned)mbi->framebuffer_bpp,
                   (unsigned)mbi->framebuffer_pitch);
        }
    }
}

static const char *
readline(const char *prefix)
{
    static char buffer[256] = {0};
    const kbd_event_t *kbd_event = NULL;
    unsigned char ascii = 0;
    size_t i = 0;

    terminal_set_color(CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_BLACK);
    printf(prefix);
    terminal_set_color(CONSOLE_COLOR_LIGHT_GREY, CONSOLE_COLOR_BLACK);
    printf("> ");

    while (1)
    {
        while (!(input_byte(0x64) & 0x01))
        {
            /* Wait for input... */
        }

        kbd_event = kbd_on_input(input_byte(0x60));
        if (kbd_event != NULL)
        {
            kbd_event_t crtl = kbd_get_key_state(VK_LCONTROL);

            if (!kbd_event->flags.is_pressed)
            {
                /* Ignore key releases. */
                continue;
            }

            if (crtl.flags.is_pressed)
            {
                if (kbd_event->vk == VK_U)
                {
                    terminal_scroll(CONSOLE_SCROLL_UP, 5);
                }
                else if (kbd_event->vk == VK_D)
                {
                    terminal_scroll(CONSOLE_SCROLL_DOWN, 5);
                }
                continue;
            }

            ascii = kbd_translate_event(kbd_event);

            if (!ascii)
            {
                continue;
            }

            if (ascii == '\n')
            {
                buffer[i] = '\0';
                putchar('\n');
                return buffer;
            }

            if (ascii == '\b')
            {
                if (i > 0)
                {
                    i--;
                    putchar('\b');
                }
                continue;
            }

            if (i < sizeof(buffer) - 1)
            {
                buffer[i++] = ascii;
                putchar(ascii);
            }
        }
    }

    return NULL;
}

void kernel_main(uint32_t magic, multiboot_info_t *mbi)
{
    const char *command = NULL;
    (void)mbi;

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        return;
    }

    puts("Type 'help' for a list of commands.\n");

    while (1)
    {
        command = readline("kfs");

        if (command && *command)
        {

            if (strcmp(command, "switch") == 0)
            {
                terminal_switch();
            }
            else if (strcmp(command, "help") == 0)
            {
                puts("switch - switch to another virtual terminal.");
            }
            else if (strcmp(command, "multiboot") == 0)
            {
                print_multiboot(mbi);
            }
            else if (strcmp(command, "printstk") == 0)
            {

            }
            else if (strcmp(command, "exit") == 0)
            {
                puts("Exiting...");
                break;
            }
            else if (strcmp(command, "reboot") == 0)
            {
                puts("Rebooting...");
                uint8_t good = 0x02;
                while (good & 0x02)
                {
                    good = input_byte(0x64);
                }
                output_byte(0x64, 0xFE);
            }
            else
            {
                printf("command not found: %s\n", command);
            }
        }
    }
}