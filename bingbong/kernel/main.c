#include "common.h"
#include "keyboard.h"
#include "panic.h"
#include "vga.h"

int a = 5;

void kmain(void)
{
    Keyboard keyboard;
    keyboard_create(&keyboard);

    vga_clear();
    vga_set_color(ColorLightGray, ColorBlack);
    vga_put_string("Hello\n");
    while (true) {
        keyboard_update(&keyboard);
        if (keyboard_key_has_been_pressed(&keyboard)) {
            char typed_char = keyboard_active_char(&keyboard);
            vga_put_char(typed_char);
            vga_put_string(" has been pressed\n");
        }
        if (keyboard_key_has_been_released(&keyboard)) {
            char typed_char = keyboard_active_char(&keyboard);
            vga_put_char(typed_char);
            vga_put_string(" has been released\n");
        }
        keyboard_clear_press(&keyboard);
    }
}

void kpanic(void)
{
    vga_set_position(0, 0);
    vga_set_color(ColorLightRed, ColorBlack);
    vga_put_string("PANIC!!!");
}
