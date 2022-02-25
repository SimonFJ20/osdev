
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
    struct term* t = {0};
    term_init(t);
    term_writestr(t, "windows bad, linux bloat, hello from new thing\nps. russia do be invading ukraine\n\nno.\n");
}

 