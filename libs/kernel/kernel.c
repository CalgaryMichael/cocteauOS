#include <kernel/tty.h>

void boot(void) {
	terminal_init();
	terminal_out("Hello, kernel\n");
}

