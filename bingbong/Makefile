
BOOT_ASM_SRC = \
	boot/header.asm \
	boot/main.asm \
	boot/main64.asm \

BOOT_ASM_SRC_DIR = boot
BOOT_ASM_OBJ_DIR = build/boot

BOOT_ASM_OBJS = $(patsubst $(BOOT_ASM_SRC_DIR)/%.asm, $(BOOT_ASM_OBJ_DIR)/%.o, $(BOOT_ASM_SRC))

all: build

$(BOOT_ASM_OBJS): $(BOOT_ASM_OBJ_DIR)/%.o : $(BOOT_ASM_SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $(patsubst $(BOOT_ASM_OBJ_DIR)/%.o, $(BOOT_ASM_SRC_DIR)/%.asm, $@) -o $@

.PHONY: build clean

build: $(BOOT_ASM_OBJS)
	mkdir -p dist/
	x86_64-elf-ld -n -o dist/kernel.bin -T linker.ld $(BOOT_ASM_OBJS)

	cp -r iso build/
	cp dist/kernel.bin build/iso/boot/
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/kernel.iso build/iso

clean:
	rm -rf build/ dist/
