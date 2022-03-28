#ifndef IDT_KEYBOARD_H
#define IDT_KEYBOARD_H



#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define KB_ENABLE_ONLY_IRQ1_FLAGS 0xFD
#define KB_DATA_PORT 0x60
#define KB_STATUS_PORT 0x64
#define ENTER_KEY_CODE 0x1C

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

void load_idt(unsigned long *idt_ptr);
void idt_init(struct IDT_entry* IDT);

struct KBEventHandler {
	void (*handler)(char keycode);
};

extern unsigned char keyboard_map[128];

void kb_init(struct KBEventHandler* handler);
void kb_event_interrupt(void);
void kb_event_interrupt_handler(void);

#endif