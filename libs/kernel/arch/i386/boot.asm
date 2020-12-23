;boot.asm
bits 32

MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot


; Define necessary variables for this to be considered a "multiboot" by GRUB
section .multiboot
align 4
        dd MAGIC
        dd FLAGS
        dd CHECKSUM


section .bss
align 16
resb 16384			;16KB for stack
stack_space:


; Kernel entry point
section .text
global _start:function (_start.end - _start)
_start:
	mov esp, stack_space	;set stack pointer

	extern _init
	call _init		;call global constructor

	extern boot		;boot is defined in kernel.c
	call boot

	cli 			;block interrupts
.hang:	hlt
	jmp .hang
.end:

