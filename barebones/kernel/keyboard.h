#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "util.h"
#include "vga.h"

void keyboard_init(struct term* t);
void keyboard_init_idt();
void handle_keyboard_interrupt();

#endif
