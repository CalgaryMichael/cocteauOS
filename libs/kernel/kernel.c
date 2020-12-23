#include <kernel/tty.h>

void kmain(void) {
	terminal_init();
	terminal_out("Hello, kernel\n");
}

