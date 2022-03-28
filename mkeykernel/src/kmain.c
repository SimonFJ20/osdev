/*
* Copyright (C) 2014  Arjun Sreedharan
* License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
*/
#include "idt_keyboard.h"
#include "keyboard_map.h"
#include "vga_screen.h"

struct MyKBEventHandlerArgs {
    struct VGATerm* term;
};

struct MyKBEventHandler {
    void (*handler)(char keycode, struct MyKBEventHandlerArgs* args);
    struct MyKBEventHandlerArgs args;
};

void keyboard_handler(char keycode, struct MyKBEventHandlerArgs* args)
{
    struct VGATerm* term = args->term;
    if (keycode == ENTER_KEY_CODE)
        return term_newline(term);
    term->buffer[term->location++] = keyboard_map[(unsigned char)keycode];
    term->buffer[term->location++] = 0x07;
}

void kmain(void)
{
    struct IDT_entry IDT[IDT_SIZE];

    struct VGATerm t = {
        .location = 0,
        .buffer = (char*)0xb8000,
    };

    struct MyKBEventHandler kbHandler = {
        .handler = &keyboard_handler,
        .args = {
            .term = &t,
        }
    };

    term_clear_screen(&t);
    term_print(&t, "Initializing kernel");
    term_newline(&t);
    term_newline(&t);

    idt_init(IDT);
    kb_init((struct KBEventHandler*)&kbHandler);

    while (1)
        ;
}
