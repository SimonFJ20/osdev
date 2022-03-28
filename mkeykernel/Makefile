
CFLAGS = -m32 -nostdlib -nolibc -ffreestanding -fno-stack-protector -Wall -Wextra -Werror

OBJECTS = \
	kernel.o \
	kmain.o \
	idt_keyboard.o \
	port.o \
	vga_screen.o \

all: kernel

run: kernel
	qemu-system-i386 -kernel kernel

kernel: $(OBJECTS) link.ld
	ld -m elf_i386 -T link.ld -o kernel $(OBJECTS)

%.o: %.c
	gcc -c $^ -o $@ $(CFLAGS)

%.o: %.asm
	nasm -f elf32 $^ -o $@

clean:
	rm -rf $(OBJECTS) kernel

