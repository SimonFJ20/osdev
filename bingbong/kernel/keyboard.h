#pragma once

#include "common.h"

typedef struct {
    u64 keys_pressed[2];
    char last_char;
    bool pressed;
    bool checked;
} Keyboard;

void keyboard_create(Keyboard*);
void keyboard_update(Keyboard*);
void keyboard_is_char_pressed(Keyboard*, char value);
bool keyboard_press_happened(Keyboard*);
char keyboard_pressed_char(Keyboard*);
void keyboard_clear_press(Keyboard*);
