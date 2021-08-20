#include <stdio.h>

#include <kernel/debug.h>
#include <kernel/tty.h>

void kmain(void) {
	terminal_init();
	
	printf("%s\n\n", "Hello Kernel");
	debug_output();
}

