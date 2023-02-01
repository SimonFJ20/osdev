#pragma once

#include "common.h"

typedef struct {
    u64 keys_pressed[2];
} Keyboard;

void keyboard_create(Keyboard*);

void keyboard_update(Keyboard*);

bool keyboard_key_was_pressed(Keyboard*);
char keyboard_pressed_char(Keyboard*);

bool keyboard_key_was_released(Keyboard*);
char keyboard_released_char(Keyboard*);
