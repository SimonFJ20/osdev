
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kbd.h"
#include "pic.h"
#include "vga.h"

#if defined(__linux__)
#error "requires ix86 cross compiler"
#endif

#if !defined(__i386__)
#error "requires ix86-elf compiler"
#endif

struct KernelKeyEventHandler
{
    void (*event)(struct KeyEventHandler* k, int8_t keycode);
    struct term* terminal;
};

void KernelKeyEventHandler_event(struct KernelKeyEventHandler* k, int8_t keycode)
{
    term_putchar(k->terminal, keyboard_map[keycode]);
}

void kmain(void)
{
    struct term* t = {0};
    term_init(t);
    term_writestr(t, "hello world\n");
    term_color(t, vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    term_writestr(t, "loading interrupts...\n");
    struct IDT_entry idt[IDT_SIZE];
    idt_init(idt);
    term_writestr(t, "loading keyboard...\n");
    kb_init();
}

 