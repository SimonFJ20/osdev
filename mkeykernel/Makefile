
# LD = ld
# CC = gcc

LD = i686-elf-ld
CC = i686-elf-gcc

CFLAGS = -O2 -m32 -nostdlib -nolibc -ffreestanding -fno-stack-protector -Wall -Wextra -Werror

OBJECTS = \
	src/kernel.o \
	src/kmain.o \
	src/idt_keyboard.o \
	src/port.o \
	src/vga_screen.o \

all: kernel

run: kernel
	qemu-system-i386 -kernel kernel

kernel: $(OBJECTS) link.ld
	$(LD) -m elf_i386 -T link.ld -o kernel $(OBJECTS)

%.o: %.c
	$(CC) -c $^ -o $@ $(CFLAGS)

%.o: %.asm
	nasm -f elf32 $^ -o $@

clean:
	rm -rf $(OBJECTS) kernel

