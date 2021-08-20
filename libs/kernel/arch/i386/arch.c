#include <stdio.h>

#include "segmentation/segmentation.h"

void arch_init() {
	puts("Initializing i386 arch...");
	initialize_segments();
}
