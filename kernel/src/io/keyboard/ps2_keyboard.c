#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <kernel/io/keyboard/ps2_keyboard.h>

#define SCAN_CODE_SET_1_EXTENDED 0xE0
#define IS_BREAK_CODE(scancode) ((scancode & 0x80) != 0)

typedef enum kbd_state_e
{
    NORMAL,
    EXTENDED,

    EXTENDED_PRINT_MAKE_BYTE3,
    EXTENDED_PRINT_MAKE_BYTE4,

    EXTENDED_PRINT_BREAK_BYTE3,
    EXTENDED_PRINT_BREAK_BYTE4,
} kbd_driver_state_t;

static kbd_event_t kbd_state[256] = {0};
static kbd_driver_state_t state = NORMAL;
static const kbd_layout_t *kbd_layout = NULL;

void kbd_init(const kbd_layout_t *layout)
{
    /* TODO: set scan code 1. */

    kbd_layout = layout;
}

kbd_event_t kbd_get_key_state(uint8_t vk)
{
    return kbd_state[vk];
}

const kbd_event_t *kbd_on_input(uint8_t scancode)
{
    uint8_t vk = 0;

    switch (state)
    {
    case NORMAL:
        if (scancode == SCAN_CODE_SET_1_EXTENDED)
        {
            state = EXTENDED;
        }
        else
        {
            vk = kbd_layout->vk_map[scancode & 0x7F];

            kbd_state[vk].scancode = scancode;
            kbd_state[vk].vk = vk;
            kbd_state[vk].flags.is_pressed = !IS_BREAK_CODE(scancode);
            kbd_state[vk].flags.is_extended = false;
        }
        break;
    case EXTENDED:
        if (scancode == 0x2A)
        {
            state = EXTENDED_PRINT_MAKE_BYTE3;
        }
        else if (scancode == 0xB7)
        {
            state = EXTENDED_PRINT_BREAK_BYTE3;
        }
        else
        {
            vk = kbd_layout->extended_vk_map[scancode & 0x7F];
            kbd_state[vk].vk = vk;
            kbd_state[vk].scancode = scancode;
            kbd_state[vk].flags.is_pressed = !IS_BREAK_CODE(scancode);
            kbd_state[vk].flags.is_extended = true;

            state = NORMAL;
        }
        break;
    case EXTENDED_PRINT_MAKE_BYTE3:
        if (scancode == 0xE0)
        {
            state = EXTENDED_PRINT_MAKE_BYTE4;
        }
        else
        {
            state = NORMAL;
        }
        break;
    case EXTENDED_PRINT_MAKE_BYTE4:
        if (scancode == 0x37)
        {
            vk = kbd_layout->extended_vk_map[scancode];

            kbd_state[vk].vk = vk;
            kbd_state[vk].scancode = scancode;
            kbd_state[vk].flags.is_pressed = true;
            kbd_state[vk].flags.is_extended = true;
        }
        state = NORMAL;
        break;
    case EXTENDED_PRINT_BREAK_BYTE3:
        if (scancode == 0xE0)
        {
            state = EXTENDED_PRINT_BREAK_BYTE4;
        }
        else
        {
            state = NORMAL;
        }
        break;
    case EXTENDED_PRINT_BREAK_BYTE4:
        if (scancode == 0xAA)
        {
            vk = kbd_layout->extended_vk_map[scancode];

            kbd_state[vk].vk = vk;
            kbd_state[vk].scancode = scancode;
            kbd_state[vk].flags.is_pressed = false;
            kbd_state[vk].flags.is_extended = true;
        }
        state = NORMAL;
    default:
        break;
    }

    if (vk != 0)
    {
        return &kbd_state[vk];
    }

    return NULL;
}

unsigned char kbd_translate_event(const kbd_event_t *event)
{
    if (event == NULL)
    {
        return 0;
    }

    return kbd_layout->translate_ascii(kbd_state, event);
}