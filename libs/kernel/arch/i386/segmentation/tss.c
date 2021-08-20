#include <string.h>

#include "tss.h"
#include "segmentation.h"

tss_entry_t tss;

tss_entry_t initialize_tss(segment_descriptor_t kernel_data) {
	memset(&tss, 0, sizeof(tss));
	tss.ss0 = (uint32_t) &kernel_data;
	tss.esp0 = 0x00;
	return tss;
}

