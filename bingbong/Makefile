
BOOT_ASM_SRC_FILES = \
	header.asm \
	main.asm \
	main64.asm \

BOOT_ASM_SRC_DIR = boot
BOOT_ASM_OBJ_DIR = build/boot
BOOT_ASM_SRC = $(patsubst %.asm, $(BOOT_ASM_SRC_DIR)/%.asm, $(BOOT_ASM_SRC_FILES))
BOOT_ASM_OBJS = $(patsubst $(BOOT_ASM_SRC_DIR)/%.asm, $(BOOT_ASM_OBJ_DIR)/%.o, $(BOOT_ASM_SRC))

KERNEL_C_SRC_FILES = \
	main.c \
	print.c \

KERNEL_C_SRC_DIR = kernel
KERNEL_C_OBJ_DIR = build/kernel
KERNEL_C_SRC = $(patsubst %.c, $(KERNEL_C_SRC_DIR)/%.c, $(KERNEL_C_SRC_FILES))
KERNEL_C_OBJS = $(patsubst $(KERNEL_C_SRC_DIR)/%.c, $(KERNEL_C_OBJ_DIR)/%.o, $(KERNEL_C_SRC))

OBJS = $(BOOT_ASM_OBJS) $(KERNEL_C_OBJS)

all: build

build: $(OBJS)
	mkdir -p dist/
	x86_64-elf-ld -n -o dist/kernel.bin -T linker.ld $(OBJS)

	cp -r iso build/
	cp dist/kernel.bin build/iso/boot/
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/kernel.iso build/iso

$(BOOT_ASM_OBJS): $(BOOT_ASM_OBJ_DIR)/%.o : $(BOOT_ASM_SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $(patsubst $(BOOT_ASM_OBJ_DIR)/%.o, $(BOOT_ASM_SRC_DIR)/%.asm, $@) -o $@

$(KERNEL_C_OBJS): $(KERNEL_C_OBJ_DIR)/%.o : $(KERNEL_C_SRC_DIR)/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -ffreestanding $(patsubst $(KERNEL_C_OBJ_DIR)/%.o, $(KERNEL_C_SRC_DIR)/%.c, $@) -o $@

.PHONY: clean

clean:
	rm -rf build/ dist/
