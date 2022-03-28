#include "vga_screen.h"

void term_print(struct VGATerm* term, const char* str)
{
    for (unsigned int i = 0; str[i] != '\0'; i++) {
        term->buffer[term->location++] = str[i];
        term->buffer[term->location++] = 0x07;
    }
}

void term_insert_line(struct VGATerm* term)
{
    unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
    term->location += (line_size - term->location % (line_size));
}

void term_clear_screen(struct VGATerm* term)
{
    unsigned int i = 0;
    while (i < SCREENSIZE) {
        term->buffer[i++] = ' ';
        term->buffer[i++] = 0x07;
    }
}

