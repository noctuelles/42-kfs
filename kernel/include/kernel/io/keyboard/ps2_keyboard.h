#ifndef _PS2_KEYBOARD_H
#define _PS2_KEYBOARD_H

#include <stdint.h>

typedef struct kbd_event_flags_s
{
    uint8_t is_pressed : 1;
    uint8_t is_extended : 1;
} kbd_event_flags_t;

typedef struct kbd_event_s
{
    uint8_t vk;
    uint8_t scancode;
    kbd_event_flags_t flags;
} kbd_event_t;

typedef struct kbd_layout_s
{
    uint8_t vk_map[256];
    uint8_t extended_vk_map[256];

    unsigned char (*translate_ascii)(const kbd_event_t kbd_state[256], const kbd_event_t *kbd_event);
} kbd_layout_t;

kbd_event_t kbd_get_key_state(uint8_t vk);
void kbd_init(const kbd_layout_t *layout);
const kbd_event_t *kbd_on_input(uint8_t scancode);
unsigned char kbd_translate_event(const kbd_event_t *event);

#endif