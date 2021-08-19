#include <stdio.h>

#include <kernel/debug.h>

void kmain(void) {
	terminal_init();
	
	terminal_out("Hello, kernel\n\n\0");
	debug_output();
	terminal_out("\n\n\0");
}

