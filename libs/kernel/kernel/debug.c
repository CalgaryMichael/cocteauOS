#include <stdio.h>

#include <kernel/tty.h>

static const char CHARACTER_ARRAY[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const int NEWLINE_INCREMENT = 6;

void debug_output(void) {
	puts("Debug Output\n\0");
	puts("******************\0");

	for (size_t i = 0; i < sizeof(CHARACTER_ARRAY); i++) {
		if (i % NEWLINE_INCREMENT == 0) {
			terminal_out("\n\n\0");
		}
		terminal_out("\t\0");
		putchar(CHARACTER_ARRAY[i]);
		terminal_out("\t\0");
	}
}

