#include "vgaterm.h"
#include "util.h"

void term_init(struct term* t)
{
    *t = (struct term) {
        .row = 0,
        .column = 0,
        .color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK),
        .buffer = (uint16_t*) 0xB8000,
    };
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            t->buffer[y * VGA_WIDTH + x] = vga_entry(' ', t->color);
}
    
void term_color(struct term* t, uint8_t color)
{
    t->color = color;
}

void term_putentryat(struct term* t, char c, uint8_t color, size_t x, size_t y)
{
    t->buffer[y * VGA_WIDTH + x] = vga_entry(c, color);
}

void term_putchar(struct term* t, char c)
{
    switch (c)
    {
        case '\n':
            t->column = 0;
            t->row++;
            return;
        case '\t':
            t->column += 4;
            if (t->column >= VGA_WIDTH)
            {
                t->column = 0;
                t->row++;
            }
    }

    term_putentryat(t, c, t->color, t->column, t->row);
    t->column++;
    if (t->column == VGA_WIDTH)
    {
        t->column = 0;
        t->row++;
        if (t->row == VGA_HEIGHT)
            t->row = 0;
    }
}

void term_write(struct term* t, const char* str, size_t len)
{
    for (size_t i = 0; i < len; i++)
        term_putchar(t, str[i]);
}

void term_writestr(struct term* t, const char* str)
{
    term_write(t, str, strlen(str));
}


