#include <stddef.h>
#include <stdio.h>
#include <string.h>
 
int puts(const char* string) {
	size_t i = 0;
	while (string[i]) {
		if (putchar(string[i]) == EOF) {
			return EOF;
		}
		i++;
	}
	if (putchar('\n') == EOF) {
		return EOF;
	}
	return 1;
}

