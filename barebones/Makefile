
all: os.iso

run: os.iso
	qemu-system-i386 -cdrom os.iso

run-kernel: kernel.bin
	qemu-system-i386 -kernel kernel.bin

os.iso: kernel.bin
	mkdir -p target/boot/grub
	cp kernel.bin target/boot/kernel.bin
	cp grub.cfg target/boot/grub/grub.cfg
	grub-mkrescue -o $@ target

KERNEL_CFLAGS=-Ikernel -O2 -Wall -Wextra -ffreestanding
KERNEL_ASMFLAGS=-Ikernel
KERNEL_LFLAGS=-nostdlib -lgcc

KERNEL_OBJS=kernel/kernel.o \
			kernel/util.o \
			kernel/vga.o \
			kernel/ports.o \
			kernel/keyboard.o 

kernel.bin: boot.o linker.ld $(KERNEL_OBJS)
	i686-elf-gcc -T linker.ld -o $@ boot.o $(KERNEL_OBJS) $(KERNEL_LFLAGS) $(KERNEL_CFLAGS)
	grub-file --is-x86-multiboot $@

kernel/%.o: kernel/%.c
	i686-elf-gcc -c $^ -o $@ -std=gnu99 $(KERNEL_CFLAGS)

kernel/%.o: kernel/%.asm
	nasm -f elf32 $^ -o $@ $(KERNEL_ASMFLAGS)

boot.o: boot.s
	i686-elf-as boot.s -o $@

clean:
	$(RM) -r target
	$(RM) os.iso
	$(RM) kernel.bin
	$(RM) boot.o
	$(RM) $(KERNEL_OBJS)

