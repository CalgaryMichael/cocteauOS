#include <stdint.h>

#include <stdio.h>

#include <kernel/segmentation.h>
#include <kernel/tty.h>

// defined in arch/*/gdt.asm
extern void set_gdt(uint64_t flags, size_t size);

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

void create_descriptor(struct SegmentDescriptor descriptor, int verbose) {
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

	if (verbose) {
		printf("First DW:  0x%08x\n", first_dw);
		printf("Second DW: 0x%08x\n", second_dw);
	}

	uint64_t output = ((uint64_t)second_dw << 32) + first_dw;

	set_gdt(output, sizeof(output));
	return;
}

void null_gdt() {
	terminal_out("Setting null GDT...\n\0");
	struct SegmentDescriptor gdt_code_null = {
		.base            = 0x00000000,
		.limit           = 0x00000000,
		.segment_type    = 0,
		.descriptor_type = 0,
		.privilege_level = 0,
		.present         = 0,
		.operation_size  = 0,
		.granularity     = 0,
	};

	create_descriptor(gdt_code_null, 0);
}

void kernel_gdt() {
	terminal_out("Setting Kernel level GDT...\n\0");
	struct SegmentDescriptor gdt_code_pl0 = {
		.base            = 0x00000000,
		.limit           = 0x000FFFFF,
		.segment_type    = EXECUTE_READ,
		.descriptor_type = CODE_DATA,
		.privilege_level = KERNEL,
		.present         = IN_MEMORY,
		.operation_size  = BITS_32,
		.granularity     = KILOBYTE,
	};
	
	struct SegmentDescriptor gdt_data_pl0 = {
		.base            = 0x00000000,
		.limit           = 0x000FFFFF,
		.segment_type    = READ_WRITE,
		.descriptor_type = CODE_DATA,
		.privilege_level = KERNEL,
		.present         = IN_MEMORY,
		.operation_size  = BITS_32,
		.granularity     = KILOBYTE,
	};
	
	create_descriptor(gdt_code_pl0, 1);
	create_descriptor(gdt_data_pl0, 1);
}

void initialize_segments() {
	null_gdt();
	kernel_gdt();
	//device_gdt();
	//applications_gdt();
}

