#include <stddef.h>
#include <stdio.h>
#include <string.h>
 
int puts(const char* string) {
	size_t i = 0;
	while (i < strlen(string)) {
		putchar(string[i]);
		i++;
	}
	return i;
}

