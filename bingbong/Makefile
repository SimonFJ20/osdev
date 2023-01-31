
BOOT_ASM_SRC_FILES = \
	header.asm \
	start.asm \
	start64.asm \

BOOT_ASM_SRC_DIR = boot
BOOT_ASM_OBJ_DIR = build/boot
BOOT_ASM_SRC = $(patsubst %.asm, $(BOOT_ASM_SRC_DIR)/%.asm, $(BOOT_ASM_SRC_FILES))
BOOT_ASM_OBJS = $(patsubst $(BOOT_ASM_SRC_DIR)/%.asm, $(BOOT_ASM_OBJ_DIR)/%.o, $(BOOT_ASM_SRC))

KERNEL_C_SRC_FILES = \
	main.c \
	vga.c \

KERNEL_C_SRC_DIR = kernel
KERNEL_C_OBJ_DIR = build/kernel
KERNEL_C_SRC = $(patsubst %.c, $(KERNEL_C_SRC_DIR)/%.c, $(KERNEL_C_SRC_FILES))
KERNEL_C_OBJS = $(patsubst $(KERNEL_C_SRC_DIR)/%.c, $(KERNEL_C_OBJ_DIR)/%.o, $(KERNEL_C_SRC))

OBJS = $(BOOT_ASM_OBJS) $(KERNEL_C_OBJS)

LINKER_SCRIPT = linker.ld

KERNEL_BIN = dist/kernel.bin
KERNEL_ISO = dist/kernel.iso

C_FLAGS = -masm=intel -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow
C_DEV_FLAGS = $(C_FLAGS) -xc -Wno-empty-translation-unit

all: build

devenv: compile_flags.txt

build: $(OBJS)
	mkdir -p dist/
	x86_64-elf-ld -n -o $(KERNEL_BIN) -T $(LINKER_SCRIPT) $(OBJS)

	cp -r iso build/
	cp $(KERNEL_BIN) build/iso/boot/
	grub-mkrescue /usr/lib/grub/i386-pc -o $(KERNEL_ISO) build/iso

$(BOOT_ASM_OBJS): $(BOOT_ASM_OBJ_DIR)/%.o : $(BOOT_ASM_SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $(patsubst $(BOOT_ASM_OBJ_DIR)/%.o, $(BOOT_ASM_SRC_DIR)/%.asm, $@) -o $@

$(KERNEL_C_OBJS): $(KERNEL_C_OBJ_DIR)/%.o : $(KERNEL_C_SRC_DIR)/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -ffreestanding $(patsubst $(KERNEL_C_OBJ_DIR)/%.o, $(KERNEL_C_SRC_DIR)/%.c, $@) -o $@ -masm=intel

compile_flags.txt:
	echo -xc++ $(C_DEV_FLAGS) | sed 's/\s\+/\n/g' > compile_flags.txt

.PHONY: clean

clean:
	rm -rf build/ dist/
