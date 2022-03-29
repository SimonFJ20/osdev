
/* define multiboot params */
.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

/* set multiboot params in multiboot sector */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* setup stack in bss sector */
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

/* entry point from bootloader */
.section .text
.global _start
.type _start, @function
_start:
    cli
    mov $stack_top, %esp # set sp
    call kmain
    hlt

.size _start, . - _start

