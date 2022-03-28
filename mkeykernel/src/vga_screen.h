#ifndef VGA_SCREEN_H
#define VGA_SCREEN_H

/* there are 25 lines each of 80 columns; each element takes 2 bytes */
#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

struct VGATerm {
    int location;
    char* buffer;
};

void term_print(struct VGATerm* term, const char *str);
void term_insert_line(struct VGATerm* term);
void term_clear_screen(struct VGATerm* term);

#endif