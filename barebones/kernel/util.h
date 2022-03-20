#ifndef UTIL_H
#define UTIL_H

#include "vga.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);
char* strncpy(char* dest, const char* src, size_t n);

#define I32_DEC_STR_LEN 12
void i32_dec_str(char* result, const int32_t value);

#define U32_HEX_STR_LEN 9
void u32_hex_str(char* result, const uint32_t value);

#endif
