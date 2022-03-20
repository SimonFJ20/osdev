
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vga.h"

#if defined(__linux__)
#error "requires ix86 cross compiler"
#endif

#if !defined(__i386__)
#error "requires ix86-elf compiler"
#endif

void kmain(void)
{
    struct term* t = { 0 };
    term_init(t);
    term_writestr(t, "hello world\n");
    term_color(t, vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    term_writestr(t, "hacking...\n");
    for (int i = 0; i < 20; i++) {
        if (i % 2) {
            term_writeint(t, i);
        } else {
            term_writestr(t, "0x");
            term_writehex(t, i);
        }
        term_linefeed(t);
    }
}
