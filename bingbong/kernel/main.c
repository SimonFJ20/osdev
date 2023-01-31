#include "common.h"
#include "panic.h"
#include "vga.h"

int a = 5;

void kmain(void)
{
    vga_clear();
    vga_set_color(ColorYellow, ColorBlack);
    vga_put_string("hacking...\n");
    panic();
}

void panic(void)
{
    vga_set_position(0, 0);
    vga_set_color(ColorRed, ColorBlack);
    // clang-format off
    vga_put_string("                                      \n");
    vga_put_string("  ############################################################################  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                  PANIC!!!                                #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  #                                                                          #  \n");
    vga_put_string("  ############################################################################  \n");
    vga_put_string("                                      \n");
    // clang-format one
}
