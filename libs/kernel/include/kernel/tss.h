#ifndef _KERNEL_TSS_H
#define _KERNEL_TSS_H

#include <kernel/segmentation.h>

struct TaskStateSegmentEntry {
	uint32_t prev_tss;

	// loaded when changing to R0 (Kernel mode)
	uint32_t esp0;		// stack pointer
	uint32_t ss0;		// stack segment
	
	// loaded when changing to R1 (High Device mode) 
	uint32_t esp1;		// stack pointer
	uint32_t ss1;		// stack segment
	
	// loaded when changing to R2 (Low Device mode) 
	uint32_t esp2;		// stack pointer
	uint32_t ss2;		// stack segment

	uint32_t cr3;		// control state registers
	uint32_t eip;		// instruction pointer
	uint32_t eflags;	// flags register

	// general registers
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;

	// segment registers
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
} __attribute__((packed));

typedef struct TaskStateSegmentEntry tss_entry_t;

tss_entry_t initialize_tss(segment_descriptor_t kernel_data);

#endif

