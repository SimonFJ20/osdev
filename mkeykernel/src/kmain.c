/*
* Copyright (C) 2014  Arjun Sreedharan
* License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
*/
#include "keyboard_map.h"
#include "vga_screen.h"
#include "idt_keyboard.h"

void keyboard_handler(char keycode)
{
    if(keycode == ENTER_KEY_CODE)
        return kprint_newline();

    vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
    vidptr[current_loc++] = 0x07;
}

void kmain(void)
{
    struct IDT_entry IDT[IDT_SIZE];

    struct KBEventHandler kbEventHandler = {
        .handler = keyboard_handler,
    };

    clear_screen();
    kprint("Initializing kernel");
    kprint_newline();
    kprint_newline();

    idt_init(IDT);
    kb_init(&kbEventHandler);

    while(1);
}
