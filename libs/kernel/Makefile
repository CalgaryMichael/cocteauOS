DESTDIR ?=
PREFIX ?= /Users/cm/repos/cocteauOS/build
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot

# Define target architecture
HOST ?= i686-elf
HOST_ARCH := $(shell ../../bin/target-triplet-to-arch $(HOST))
ARCHDIR = arch/$(HOST_ARCH)

include $(ARCHDIR)/make.config


ASFLAGS := $(ASFLAGS) $(KERNEL_ARCH_ASFLAGS)

CFLAGS ?= -O2 -g
CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra $(KERNEL_ARCH_CFLAGS)

LIBS ?=
LIBS := $(LIBS) -nostdlib -lgcc $(KERNEL_ARCH_LIBS)


KERNEL_OBJS = \
	    $(KERNEL_ARCH_OBJS) \
	    kernel.o \

OBJS = \
     $(KERNEL_OBJS)\

LINK_LIST = \
	  $(KERNEL_OBJS) \
	  $(LIBS)


.PHONY: all clean install-kernel
.SUFFIXES: .o .c .asm


.c.o:
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS)

.asm.o:
	$(AS) $(ASFLAGS) $< -o $@


myos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot myos.kernel

install-kernel: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)


all: myos.kernel
install: install-kernel
clean:
	rm -f myos.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d


-include $(OBJS:.o=.d)

