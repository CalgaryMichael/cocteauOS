OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

__KERNEL_START = 1M;


SECTIONS {
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = __KERNEL_START;

	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.text ALIGN(4K) : {
		*(.multiboot)
		*(.gdt)
		*(.text)

		_etext = .;
	}

	/* Read-only data. */
	.rodata ALIGN(4K) : {
		*(.rodata)

		_erodata = .;
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : {
		_sdata = .;

		*(.data)

		_edata = .;
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) (NOLOAD) : {
		_sbss = .;

		*(.bss)
		*(COMMON)

		_ebss = .;
	}

	__KERNEL_END = .;
}

