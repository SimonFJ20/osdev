
ASM_SRC = \
	src/header.asm \
	src/main.asm \

ASM_OBJS = $(patsubst src/%.asm, build/obj/%.o, $(ASM_SRC))

all: build

$(ASM_OBJS): build/obj/%.o : src/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $(patsubst build/obj/%.o, src/%.asm, $@) -o $@

.PHONY: build clean

build: $(ASM_OBJS)
	mkdir -p dist/
	x86_64-elf-ld -n -o dist/kernel.bin -T linker.ld $(ASM_OBJS)

	cp -r iso build/
	cp dist/kernel.bin build/iso/boot/
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/kernel.iso build/iso

clean:
	rm -rf build/ dist/
