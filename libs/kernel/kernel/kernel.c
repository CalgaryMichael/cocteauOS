#include <stdio.h>

#include <kernel/arch.h>
#include <kernel/debug.h>
#include <kernel/tty.h>

void kmain(void) {
	terminal_init();
	
	printf("%s\n\n", "Hello Kernel");
	debug_output();
	puts("\n");

	arch_init();
}

