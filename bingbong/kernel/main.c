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
    vga_set_color(ColorYellow, ColorBlack);
    vga_put_string("Hello\n");
    while (true) {
        keyboard_update(&keyboard);
        if (keyboard_press_happened(&keyboard)) {
            char typed_char = keyboard_pressed_char(&keyboard);
            keyboard_clear_press(&keyboard);
            vga_put_char(typed_char);
        }
    }
}

void kpanic(void)
{
    vga_set_position(0, 0);
    vga_set_color(ColorLightRed, ColorBlack);
    vga_put_string("PANIC!!!");
}
