#ifndef _PS2_KEYBOARD_H
#define _PS2_KEYBOARD_H

#include <stdint.h>

#define VK_F1 0x70
#define VK_F2 0x71
#define VK_F3 0x72
#define VK_F4 0x73
#define VK_F5 0x74
#define VK_F6 0x75
#define VK_F7 0x76
#define VK_F8 0x77
#define VK_F9 0x78
#define VK_F10 0x79
#define VK_F11 0x7A
#define VK_F12 0x7B

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39

#define VK_LSHIFT 0xA0
#define VK_RSHIFT 0xA1

#define VK_CONTROL 0x11
#define VK_LCONTROL 0xA2
#define VK_RCONTROL 0xA3

#define VK_SPACE 0x20
#define VK_RETURN 0x0D
#define VK_BACKSPACE 0x08

typedef struct kbd_event_flags_s {
    uint8_t is_pressed : 1;
    uint8_t is_extended : 1;
} kbd_event_flags_t;

typedef struct kbd_event_s {
    uint8_t vk;
    uint8_t scancode;
    kbd_event_flags_t flags;
} kbd_event_t;

typedef struct kbd_layout_s {
    uint8_t vk_map[256];
    uint8_t extended_vk_map[256];

    unsigned char (*translate_ascii)(const kbd_event_t kbd_state[256], const kbd_event_t *kbd_event);
} kbd_layout_t;


kbd_event_t kbd_get_key_state(uint8_t vk);
void kbd_init(const kbd_layout_t *layout);
const kbd_event_t *kbd_on_input(uint8_t scancode);
unsigned char kbd_translate_event(const kbd_event_t *event);

#endif