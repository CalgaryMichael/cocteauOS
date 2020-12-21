;boot.asm

MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot


section .multiboot
align 4
        dd MAGIC
        dd FLAGS
        dd CHECKSUM


section .bss
align 16
resb 16384			;16KB for stack
stack_space:


section .text
global _start:function (_start.end - _start)
_start:
	cli 			;block interrupts
	mov esp, stack_space	;set stack pointer
	extern boot		;boot is defined in kernel.c
	call boot
.hang:	hlt
	jmp .hang
.end:

