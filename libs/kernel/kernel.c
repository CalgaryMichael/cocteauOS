#include <kernel/tty.h>

void kmain(void) {
	terminal_init();
	terminal_out("Hello, kernel\n");
	terminal_out("This is a new line\n");
	terminal_out("This is a\ttab");
}

