#pragma once

#include "common.h"

typedef struct {
    u64 keys_pressed[2];
    bool key_has_been_released;
    bool key_has_been_pressed;
    char active_char;
} Keyboard;

void keyboard_create(Keyboard*);
void keyboard_update(Keyboard*);
bool keyboard_is_char_pressed(Keyboard*, char value);
bool keyboard_key_has_been_released(Keyboard*);
bool keyboard_key_has_been_pressed(Keyboard*);
char keyboard_active_char(Keyboard*);
void keyboard_clear_press(Keyboard*);
