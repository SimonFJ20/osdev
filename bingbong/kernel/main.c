#include "common.h"
#include "print.h"

void kmain()
{
    print_clear();
    print_set_color(ColorYellow, ColorBlack);
    while (true) {
        print_string("weeeeeee\n");
    }
}
