#ifndef KBD_H
#define KBD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define ENTER_KEY_CODE 0x1C
#define KEYBOARD_STATUS_PORT 0x64

struct KeyEventHandler
{
    void (*event)(struct KeyEventHandler* k, int8_t keycode);
};

extern uint8_t keyboard_map[128];

void kb_init(void);

#endif