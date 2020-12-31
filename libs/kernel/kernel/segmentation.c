#include <stdint.h>

#include <stdio.h>

#include <kernel/segmentation.h>
#include <kernel/tty.h>

uint32_t create_flags(struct SegmentDescriptor descriptor) {
	uint32_t flags;
        flags  = 0x00000000;
	flags |= descriptor.segment_type << SEGMENT_TYPE_OFFSET;
	flags |= descriptor.descriptor_type << DESCRIPTOR_TYPE_OFFSET;
	flags |= descriptor.privilege_level << PRIVILEGE_LEVEL_OFFSET;
	flags |= descriptor.present << PRESENT_OFFSET;
	flags |= descriptor.operation_size << OPERATION_SIZE_OFFSET;
	flags |= descriptor.granularity << GRANULARITY_OFFSET;

	return flags;
}

void create_descriptor(struct SegmentDescriptor descriptor) {
	uint32_t flags = create_flags(descriptor);

	uint32_t first_dw;
	first_dw  = 0x00000000;
	first_dw |= descriptor.limit & LIMIT_FIRST_DW_MASK;
	first_dw |= (descriptor.base << BASE_FIRST_DW_OFFSET) & BASE_FIRST_DW_MASK;

	uint32_t second_dw;
	second_dw  = 0x00000000;
	second_dw |= flags & FLAG_MASK;                                                       // set flags: type, p, dpl, s, g, d/b, l, avl
	second_dw |= (descriptor.base >> BASE_SECOND_DW_OFFSET) & BASE_SECOND_DW_LOWER_MASK;  // set base bits 23:16
	second_dw |= descriptor.base & BASE_SECOND_DW_UPPER_MASK;                             // set base bits 31:24
	second_dw |= descriptor.limit & LIMIT_SECOND_DW_MASK;                                 // set limit bits 19:16

	printf("First DW:  0x%08x\n", first_dw);
	printf("Second DW: 0x%08x\n", second_dw);

	uint64_t output = ((uint64_t)second_dw << 32) + first_dw;

	// TODO: actually call LGDT assembly function

	return;
}

void initialize_gdt() {
	terminal_out("Initializing GDT...\n");

	struct SegmentDescriptor gdt_code_pl0 = {
		.base            = 0x00000000,
		.limit           = 0x000FFFFF,
		.segment_type    = EXECUTE_READ,
		.descriptor_type = CODE_DATA,
		.privilege_level = KERNEL,
		.present         = IN_MEMORY,
		.operation_size  = BITES_32,
		.granularity     = KILOBYTE,
	};
	
	create_descriptor(gdt_code_pl0);
}

void initialize_segments() {
	initialize_gdt();
}

