#include <stdint.h>

#include <stdio.h>

#include <kernel/segmentation.h>
#include <kernel/tss.h>
#include <kernel/tty.h>

segment_descriptor_t gdt_entries[8];

// defined in arch/*/gdt.asm
extern void set_gdt(uint64_t flags, size_t size);

uint32_t create_flags(segment_descriptor_t descriptor) {
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

void create_descriptor(segment_descriptor_t descriptor, int verbose) {
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
	segment_descriptor_t gdt_code_null = {
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
	gdt_entries[NULL_ENTRY] = gdt_code_null;
}

void kernel_gdt() {
	terminal_out("Setting Kernel level GDT...\n\0");
	segment_descriptor_t gdt_code_pl0 = {
		.base            = 0x00000000,
		.limit           = 0x000FFFFF,
		.segment_type    = EXECUTE_READ,
		.descriptor_type = CODE_DATA,
		.privilege_level = KERNEL,
		.present         = IN_MEMORY,
		.operation_size  = BITS_32,
		.granularity     = KILOBYTE,
	};
	
	segment_descriptor_t gdt_data_pl0 = {
		.base            = 0x00000000,
		.limit           = 0x000FFFFF,
		.segment_type    = READ_WRITE,
		.descriptor_type = CODE_DATA,
		.privilege_level = KERNEL,
		.present         = IN_MEMORY,
		.operation_size  = BITS_32,
		.granularity     = KILOBYTE,
	};
	
	create_descriptor(gdt_code_pl0, 0);
	create_descriptor(gdt_data_pl0, 0);

	gdt_entries[KERNEL_CODE] = gdt_code_pl0;
	gdt_entries[KERNEL_DATA] = gdt_data_pl0;
}

void tss_gdt() {
	tss_entry_t tss = initialize_tss(gdt_entries[KERNEL_DATA]);
	terminal_out("Setting TSS GDT...\n\0");
	segment_descriptor_t tss_descriptor = {
		.base            = (uint32_t) &tss,
		.limit           = sizeof(tss),
		.segment_type    = EXECUTE_ONLY_ACCESSED,
		.descriptor_type = SYSTEM,
		.privilege_level = KERNEL,
		.present         = IN_MEMORY,
		.operation_size  = BITS_16,
		.granularity     = KILOBYTE,
	};
	create_descriptor(tss_descriptor, 0);
	gdt_entries[TSS] = tss_descriptor;
}

void device_gdt() {
	terminal_out("Setting Device level GDT...\n\0");
	segment_descriptor_t gdt_code_pl1 = {
		.base            = 0x00000000,
		.limit           = 0x000FFFFF,
		.segment_type    = EXECUTE_READ,
		.descriptor_type = CODE_DATA,
		.privilege_level = DEVICE_DRIVERS_HIGH,
		.present         = IN_MEMORY,
		.operation_size  = BITS_32,
		.granularity     = KILOBYTE,
	};

	segment_descriptor_t gdt_data_pl1 = {
		.base            = 0x00000000,
		.limit           = 0x000FFFFF,
		.segment_type    = READ_WRITE,
		.descriptor_type = CODE_DATA,
		.privilege_level = DEVICE_DRIVERS_HIGH,
		.present         = IN_MEMORY,
		.operation_size  = BITS_32,
		.granularity     = KILOBYTE,
	};

	create_descriptor(gdt_code_pl1, 0);
	create_descriptor(gdt_data_pl1, 0);

	gdt_entries[DEVICE_CODE] = gdt_code_pl1;
	gdt_entries[DEVICE_DATA] = gdt_data_pl1;
}

void applications_gdt() {
	terminal_out("Setting Applications level GDT...\n\0");
	segment_descriptor_t gdt_code_pl3 = {
		.base            = 0x00000000,
		.limit           = 0x000FFFFF,
		.segment_type    = EXECUTE_READ,
		.descriptor_type = CODE_DATA,
		.privilege_level = APPLICATIONS,
		.present         = IN_MEMORY,
		.operation_size  = BITS_32,
		.granularity     = KILOBYTE,
	};

	segment_descriptor_t gdt_data_pl3 = {
		.base            = 0x00000000,
		.limit           = 0x000FFFFF,
		.segment_type    = READ_WRITE,
		.descriptor_type = CODE_DATA,
		.privilege_level = APPLICATIONS,
		.present         = IN_MEMORY,
		.operation_size  = BITS_32,
		.granularity     = KILOBYTE,
	};

	create_descriptor(gdt_code_pl3, 0);
	create_descriptor(gdt_data_pl3, 0);

	gdt_entries[APP_CODE] = gdt_code_pl3;
	gdt_entries[APP_DATA] = gdt_data_pl3;
}

void initialize_segments() {
	null_gdt();
	kernel_gdt();
	tss_gdt();
	device_gdt();
	applications_gdt();
}

