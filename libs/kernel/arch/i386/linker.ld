/*
*  link.ld
*/
ENTRY(_start)
SECTIONS
 {
   . = 1M;
   .text   : {
	*(.multiboot)
	*(.text)
   }
   .bss    : {
	*(COMMON)
	*(.bss)
   }
 }

