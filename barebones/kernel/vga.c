#include "vga.h"
#include "util.h"
#include <stdint.h>

void term_init(struct term* t)
{
    *t = (struct term) {
        .row = 0,
        .column = 0,
        .color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK),
        .buffer = (uint16_t*)VGA_BUFFER_ADDRESS,
    };
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            t->buffer[y * VGA_WIDTH + x] = vga_entry(' ', t->color);
}

void term_color(struct term* t, uint8_t color) { t->color = color; }

void term_putentryat(struct term* t, char c, uint8_t color, size_t x,
    size_t y)
{
    t->buffer[y * VGA_WIDTH + x] = vga_entry(c, color);
}

void term_linefeed(struct term* t)
{
    t->column = 0;
    t->row++;

    if (t->row >= VGA_HEIGHT) {
        for (size_t y = 0; y < VGA_HEIGHT - 1; y++)
            for (size_t x = 0; x < VGA_WIDTH; x++)
                t->buffer[y * VGA_WIDTH + x] = t->buffer[(y + 1) * VGA_WIDTH + x];
        for (size_t x = 0; x < VGA_WIDTH; x++)
            t->buffer[VGA_WIDTH * (VGA_HEIGHT - 1) + x] = vga_entry(' ', t->color);
        t->row--;
    }
}

void term_putchar(struct term* t, char c)
{
    switch (c) {
    case '\n':
        return term_linefeed(t);
    case '\t':
        t->column += 4;
        if (t->column >= VGA_WIDTH)
            term_linefeed(t);
        return;
    }

    term_putentryat(t, c, t->color, t->column, t->row);
    t->column++;
    if (t->column == VGA_WIDTH) {
        term_linefeed(t);
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

void term_writeint(struct term* t, int32_t value)
{
    char result[I32_DEC_STR_LEN] = {0};
    i32_dec_str(result, value);
    return term_writestr(t, result);
}

void term_writehex(struct term* t, uint32_t value)
{
    char result[U32_HEX_STR_LEN] = {0};
    u32_hex_str(result, value);
    return term_writestr(t, result);
}
