#ifndef PORT_H
#define PORT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

struct IDT_entry{
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
};

void idt_init(struct IDT_entry* idt);

extern void keyboard_handler(void);
extern uint8_t read_port(uint16_t port);
extern void write_port(uint16_t port, uint16_t data);
extern void load_idt(uint64_t* idt_ptr);

#endif