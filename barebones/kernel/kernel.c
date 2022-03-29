
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel.h"
#include "keyboard_map.h"

#ifdef SHOULD_COMPLAIN_ABOUT_WRONG_COMPILER

#if defined(__linux__)
#error "requires ix86 cross compiler"
#endif

#if !defined(__i386__)
#error "requires ix86-elf compiler"
#endif

#endif

struct MyKBEventHandlerArgs {
    struct term* t;
};

struct MyKBEventHandler {
    void (*handler)(char keycode, struct MyKBEventHandlerArgs* args);
    struct MyKBEventHandlerArgs args;
};

void keyboard_handler(char keycode, struct MyKBEventHandlerArgs* args)
{
    if (keycode == 0x1C)
        return term_linefeed(args->t);
    term_putchar(args->t, keyboard_map[(unsigned char)keycode]);
}

void kmain(void)
{
    struct term* t = { 0 };
    term_init(t);
    term_writestr(t, "hello world\n");
    term_color(t, vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    term_writestr(t, "hacking...\n");
    term_putchar(t, 's');

    struct IDTEntry idt[IDT_SIZE];

    struct MyKBEventHandler kbHandler = {
        .handler = &keyboard_handler,
        .args = {
            .t = t,
        }
    };

    idt_init(idt);
    kb_init((struct KBEventHandler*)&kbHandler);

    // for (int i = 0; i < 20; i++) {
    //     if (i % 2) {
    //         term_writeint(t, i);
    //     } else {
    //         term_writestr(t, "0x");
    //         term_writehex(t, i);
    //     }
    //     term_linefeed(t);
    // }
}
