#include <kernel/io/keyboard/ps2_keyboard.h>
#include <stdint.h>

static const unsigned char normal_ascii_map[256] = {
    [VK_0] = '0',
    [VK_1] = '1',
    [VK_2] = '2',
    [VK_3] = '3',
    [VK_4] = '4',
    [VK_5] = '5',
    [VK_6] = '6',
    [VK_7] = '7',
    [VK_8] = '8',
    [VK_9] = '9',
    [VK_A] = 'a',
    [VK_B] = 'b',
    [VK_C] = 'c',
    [VK_D] = 'd',
    [VK_E] = 'e',
    [VK_F] = 'f',
    [VK_G] = 'g',
    [VK_H] = 'h',
    [VK_I] = 'i',
    [VK_J] = 'j',
    [VK_K] = 'k',
    [VK_L] = 'l',
    [VK_M] = 'm',
    [VK_N] = 'n',
    [VK_O] = 'o',
    [VK_P] = 'p',
    [VK_Q] = 'q',
    [VK_R] = 'r',
    [VK_S] = 's',
    [VK_T] = 't',
    [VK_U] = 'u',
    [VK_V] = 'v',
    [VK_W] = 'w',
    [VK_X] = 'x',
    [VK_Y] = 'y',
    [VK_Z] = 'z',

    [VK_OEM_4] = '[',
    [VK_OEM_6] = ']',
    [VK_OEM_1] = ';',
    [VK_OEM_7] = '\'',
    [VK_OEM_COMMA] = ',',
    [VK_OEM_PERIOD] = '.',
    [VK_OEM_2] = '/',
    [VK_OEM_MINUS] = '-',
    [VK_OEM_PLUS] = '=',
    [VK_OEM_3] = '`',

    [VK_SPACE] = ' ',
    [VK_RETURN] = '\n',
    [VK_BACKSPACE] = '\b',
};

static const unsigned char shift_ascii_map[256] = {
    [VK_0] = ')',
    [VK_1] = '!',
    [VK_2] = '@',
    [VK_3] = '#',
    [VK_4] = '$',
    [VK_5] = '%',
    [VK_6] = '^',
    [VK_7] = '&',
    [VK_8] = '*',
    [VK_9] = '(',
    [VK_A] = 'A',
    [VK_B] = 'B',
    [VK_C] = 'C',
    [VK_D] = 'D',
    [VK_E] = 'E',
    [VK_F] = 'F',
    [VK_G] = 'G',
    [VK_H] = 'H',
    [VK_I] = 'I',
    [VK_J] = 'J',
    [VK_K] = 'K',
    [VK_L] = 'L',
    [VK_M] = 'M',
    [VK_N] = 'N',
    [VK_O] = 'O',
    [VK_P] = 'P',
    [VK_Q] = 'Q',
    [VK_R] = 'R',
    [VK_S] = 'S',
    [VK_T] = 'T',
    [VK_U] = 'U',
    [VK_V] = 'V',
    [VK_W] = 'W',
    [VK_X] = 'X',
    [VK_Y] = 'Y',
    [VK_Z] = 'Z',
    [VK_OEM_4] = '{',
    [VK_OEM_6] = '}',
    [VK_OEM_1] = ':',
    [VK_OEM_7] = '"',
    [VK_OEM_COMMA] = '<',
    [VK_OEM_PERIOD] = '>',
    [VK_OEM_2] = '?',
    [VK_OEM_MINUS] = '_',
    [VK_OEM_PLUS] = '+',
    [VK_OEM_3] = '~',
};


static unsigned char translate_ascii(const kbd_event_t kbd_state[256], const kbd_event_t *kbd_event) {
    if (!kbd_event->flags.is_pressed) {
        return 0;
    }

    if (kbd_state[VK_LSHIFT].flags.is_pressed || kbd_state[VK_RSHIFT].flags.is_pressed) {
        if (shift_ascii_map[kbd_event->vk] != 0) {
            return shift_ascii_map[kbd_event->vk];
        } else if (normal_ascii_map[kbd_event->vk] != 0) {
            return normal_ascii_map[kbd_event->vk];
        } else {
            return 0;
        }
    } else {
        return normal_ascii_map[kbd_event->vk];
    }
}

kbd_layout_t KBD_QWERTY_LAYOUT = {
    .vk_map = {
        /* Functions */
        [0x3B] = VK_F1,
        [0x3C] = VK_F2,
        [0x3D] = VK_F3,
        [0x3E] = VK_F4,
        [0x3F] = VK_F5,
        [0x40] = VK_F6,
        [0x41] = VK_F7,
        [0x42] = VK_F8,
        [0x43] = VK_F9,
        [0x44] = VK_F10,
        [0x57] = VK_F11,
        [0x58] = VK_F12,

        /* Alphabet */
        [0x1E] = VK_A,
        [0x30] = VK_B,
        [0x2E] = VK_C,
        [0x20] = VK_D,
        [0x12] = VK_E,
        [0x21] = VK_F,
        [0x22] = VK_G,
        [0x23] = VK_H,
        [0x17] = VK_I,
        [0x24] = VK_J,
        [0x25] = VK_K,
        [0x26] = VK_L,
        [0x32] = VK_M,
        [0x31] = VK_N,
        [0x18] = VK_O,
        [0x19] = VK_P,
        [0x10] = VK_Q,
        [0x13] = VK_R,
        [0x1F] = VK_S,
        [0x14] = VK_T,
        [0x16] = VK_U,
        [0x2F] = VK_V,
        [0x11] = VK_W,
        [0x2D] = VK_X,
        [0x15] = VK_Y,
        [0x2C] = VK_Z,

        /* OEM */
        [0x1A] = VK_OEM_4,
        [0x1B] = VK_OEM_6,
        [0x27] = VK_OEM_1, 
        [0x28] = VK_OEM_7,
        [0x33] = VK_OEM_COMMA,
        [0x34] = VK_OEM_PERIOD,
        [0x35] = VK_OEM_2,
        [0x0C] = VK_OEM_MINUS,
        [0x0D] = VK_OEM_PLUS,
        [0x29] = VK_OEM_3,

        /* Numbers */
        [0x02] = VK_1,
        [0x03] = VK_2,
        [0x04] = VK_3,
        [0x05] = VK_4,
        [0x06] = VK_5,
        [0x07] = VK_6,
        [0x08] = VK_7,
        [0x09] = VK_8,
        [0x0A] = VK_9,
        [0x0B] = VK_0,

        /* Keypad Numbers. */
        [0x52] = VK_NUMPAD0,
        [0x4F] = VK_NUMPAD1,
        [0x50] = VK_NUMPAD2,
        [0x51] = VK_NUMPAD3,
        [0x4B] = VK_NUMPAD4,
        [0x4C] = VK_NUMPAD5,
        [0x4D] = VK_NUMPAD6,
        [0x47] = VK_NUMPAD7,
        [0x48] = VK_NUMPAD8,
        [0x49] = VK_NUMPAD9,

        /* Controls */
        [0x2A] = VK_LSHIFT,
        [0x36] = VK_RSHIFT,
        [0x1D] = VK_LCONTROL,
        /* VK_RCONTROL is extended. */
        [0x38] = VK_LMENU,
        /* VK_LMENU is extended.*/
        [0x3A] = VK_CAPITAL,

        /* Specials */
        [0x39] = VK_SPACE,
        [0x1C] = VK_RETURN,
        [0x0E] = VK_BACKSPACE,
        [0x0F] = VK_TAB,
        [0x01] = VK_ESCAPE,
    },

    .extended_vk_map = {
        [0x1D] = VK_RCONTROL,
        [0x38] = VK_RMENU, // AltGr

        [0x5B] = VK_LGUI,
        [0x5C] = VK_RGUI,
        [0x5D] = VK_APPS,

        [0x48] = VK_UP,
        [0x50] = VK_DOWN,
        [0x4B] = VK_LEFT,
        [0x4D] = VK_RIGHT,
    },

    .translate_ascii = translate_ascii,
};
