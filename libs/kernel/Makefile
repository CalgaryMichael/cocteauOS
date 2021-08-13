DESTDIR ?=
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot
INCLUDE_DIR ?= $(PREFIX)/include

# Target architecture
HOST ?= $(shell ../../bin/default-host)
HOST_ARCH ?= $(shell ../../bin/target-triplet-to-arch $(HOST))
ARCHDIR = arch/$(HOST_ARCH)


include $(ARCHDIR)/make.config


# Flags
ASFLAGS := $(ASFLAGS) $(KERNEL_ARCH_ASFLAGS)
CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra $(KERNEL_ARCH_CFLAGS)
LDFLAGS := $(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)

LIBS := $(LIBS) -nostdlib $(KERNEL_ARCH_LIBS)


KERNEL_OBJS = \
	    $(KERNEL_ARCH_OBJS) \
	    kernel/debug.o \
	    kernel/segmentation.o \
	    kernel/tss.o \
	    kernel/kernel.o \

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


.PHONY: all clean install-headers install-kernel
.SUFFIXES: .o .c .asm


# Build Rules
.c.o:
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS)

.asm.o:
	$(AS) $(ASFLAGS) $< -o $@

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@


# Functions
cocteauOS.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -B $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot cocteauOS.kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDE_DIR)
	cp -R include/. $(DESTDIR)$(INCLUDE_DIR)/.

install-kernel: cocteauOS.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp cocteauOS.kernel $(DESTDIR)$(BOOTDIR)


# Commands
all: cocteauOS.kernel
install: install-headers install-kernel
clean:
	rm -f cocteauOS.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d


-include $(OBJS:.o=.d)

