#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
//#include <string.h>

#include <kernel/tty.h>

static char *vidptr = (char*)0xb8000; 	//video mem begins here.

void terminal_init(void) {
	unsigned int i = 0;

	/* this loops clears the screen
	* there are 25 lines each of 80 columns; each element takes 2 bytes */
	while(i < 80 * 25 * 2) {
		/* blank character */
		vidptr[i] = ' ';
		/* attribute-byte - light grey on black screen */
		vidptr[i+1] = 0x07; 		
		i = i + 2;
	}

	return;
}

void terminal_putchar(char c) {
	return;
}

void terminal_out(const char* data) {
	unsigned int i = 0;
	unsigned int j = 0;
	/* this loop writes the string to video memory */
	while (data[j] != '\0') {
		/* the character's ascii */
		vidptr[i] = data[j];
		/* attribute-byte: give character black bg and light grey fg */
		vidptr[i+1] = 0x07;
		++j;
		i = i + 2;
	}
	return;
}

