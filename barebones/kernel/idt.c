#include "pic.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void idt_init(struct IDT_entry *idt)
{
    uint64_t keyboard_address = (uint64_t) keyboard_handler;
    idt[0x21] = (struct IDT_entry) {
        .offset_lowerbits = keyboard_address & 0xFFFF,
        .selector = KERNEL_CODE_SEGMENT_OFFSET,
        .zero = 0,
        .type_attr = INTERRUPT_GATE,
        .offset_higherbits = (keyboard_address & 0xFFFF0000) >> 16,
    };
    write_port(0x20, 0x11);
    write_port(0x0A, 0x11);
    write_port(0x21, 0x20);
    write_port(0xA1, 0x28);
    write_port(0x21, 0x00);
    write_port(0xA1, 0x00);
    write_port(0x21, 0x01);
    write_port(0xA1, 0x01);
    write_port(0x21, 0xFF);
    write_port(0xA1, 0xFF);
    uint64_t idt_address = (uint64_t) idt;
    uint64_t idt_ptr[2];
    idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xFFFF) << 16);
    idt_ptr[1] = idt_address >> 16;
    load_idt(idt_ptr);
}
