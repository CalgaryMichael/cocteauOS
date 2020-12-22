DESTDIR ?=
PREFIX ?= /Users/cm/repos/cocteauOS/build
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot

# Target architecture
HOST ?= i686-elf
HOST_ARCH := $(shell ../../bin/target-triplet-to-arch $(HOST))
ARCHDIR = arch/$(HOST_ARCH)

include $(ARCHDIR)/make.config


# Flags
ASFLAGS := $(ASFLAGS) $(KERNEL_ARCH_ASFLAGS)

CFLAGS ?= -O2 -g
CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra $(KERNEL_ARCH_CFLAGS)

LDFLAGS := $(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)

LIBS ?=
LIBS := $(LIBS) -nostdlib -lgcc $(KERNEL_ARCH_LIBS)


KERNEL_OBJS = \
	    $(KERNEL_ARCH_OBJS) \
	    kernel.o \

OBJS = \
       $(ARCHDIR)/crti.o \
       $(ARCHDIR)/crtbegin.o \
       $(KERNEL_OBJS)\
       $(ARCHDIR)/crtend.o \
       $(ARCHDIR)/crtn.o \

LINK_LIST = \
	    $(ARCHDIR)/crti.o \
	    $(ARCHDIR)/crtbegin.o \
	    $(KERNEL_OBJS) \
	    $(LIBS) \
	    $(ARCHDIR)/crtend.o \
	    $(ARCHDIR)/crtn.o \


.PHONY: all clean install-kernel
.SUFFIXES: .o .c .asm


# Build Rules
.c.o:
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS)

.asm.o:
	$(AS) $(ASFLAGS) $< -o $@

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@


# Functions
myos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot myos.kernel

install-kernel: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)


# Commands
all: myos.kernel
install: install-kernel
clean:
	rm -f myos.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d


-include $(OBJS:.o=.d)

