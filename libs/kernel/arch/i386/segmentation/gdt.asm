section .gdt

gdtr DW 0  ; For limit storage
     DD 0  ; For base storage

global set_gdt:function (set_gdt.end - set_gdt)
set_gdt:
	mov	EAX, [ESP + 4]
	mov	[gdtr + 2], EAX
	mov	AX, [ESP + 8]
	mov	[gdtr], AX
	lgdt	[gdtr]
.end:
	ret

