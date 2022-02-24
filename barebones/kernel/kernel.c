
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vgaterm.h"

#if defined(__linux__)
#error "requires ix86 cross compiler"
#endif

#if !defined(__i386__)
#error "requires ix86-elf compiler"
#endif

void kernel_main(void)
{
    struct term* t = {0};
    term_init(t);
    term_writestr(t, "windows bad, linux bloat, hello from new thing\nps. fuck maksim\n");
}

