#include <stdio.h>
#include <kernel/tty.h>

void kmain(void) {
	terminal_init();
	
	terminal_out("Hello, kernel\n\0");
	terminal_out("This is a new line\n\0");
	terminal_out("This is a\ttab\n\0");
	printf("Here are some numbers: %d - %d - %d - %d - %d - %d - %d - %d - %d - %d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
}

