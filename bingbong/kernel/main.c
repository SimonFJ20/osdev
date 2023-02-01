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
    while (true) {
        keyboard_update(&keyboard);
        for (usize i = 0; i < 64; ++i)
            vga_put_char(keyboard.keys_pressed[0] & (1 << i) ? '1' : '0');
        for (usize i = 0; i < 64; ++i)
            vga_put_char(keyboard.keys_pressed[1] & (1 << i) ? '1' : '0');
        vga_linefeed();
    }
}

void kpanic(void)
{
    vga_set_position(0, 0);
    vga_set_color(ColorLightRed, ColorBlack);
    vga_put_string("PANIC!!!");
}
