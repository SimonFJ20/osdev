
# LD = ld
# CC = gcc

LD = i686-elf-ld
CC = i686-elf-gcc

CFLAGS = -O2 -std=c17 -m32 -nostdlib -nolibc -ffreestanding -fno-stack-protector -Wall -Wextra -Werror

OFILES = \
    kernel.o \
    kmain.o \
    idt_keyboard.o \
    port.o \
    vga_screen.o \

HEADERS = $(wildcard src/*.h)
OBJECTS = $(patsubst %, src/%, $(OFILES))

all: kernel

run: kernel
	qemu-system-i386 -kernel kernel

kernel: $(OBJECTS) link.ld
	$(LD) -m elf_i386 -T link.ld -o kernel $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.asm
	nasm -f elf32 $^ -o $@

clean:
	rm -rf $(OBJECTS) kernel

