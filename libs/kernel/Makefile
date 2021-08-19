HOST ?= i386
ARCHDIR = arch/$(HOST)

include $(ARCHDIR)/make.config


.PHONY: install-headers install-libk install-kernel
.SUFFIXES: .o .c .asm

ROOTDIR ?=
BOOTDIR ?=
PREFIX := $(ROOTDIR)/usr
INCLUDEDIR = $(PREFIX)/include

# Flags
CFLAGS = --sysroot=$(ROOTDIR) -isystem=$(INCLUDEDIR) -I$(INCLUDEDIR) -O2 -g -ffreestanding -Wall -Wextra $(KERNEL_ARCH_CFLAGS)
ASFLAGS = $(KERNEL_ARCH_ASFLAGS)
LDFLAGS = $(KERNEL_ARCH_LDFLAGS)

# Obj files
LIBS := $(LIBS) -nostdlib -lgcc -lk $(KERNEL_ARCH_LIBS)

KERNEL_OBJS = \
       $(KERNEL_ARCH_OBJS) \
       kernel/debug.o \
       kernel/kernel.o

OBJS = \
       $(ARCHDIR)/crti.o \
       $(ARCHDIR)/crtbegin.o \
       $(KERNEL_OBJS)\
       $(ARCHDIR)/crtend.o \
       $(ARCHDIR)/crtn.o

LINK_LIST = \
	    $(ARCHDIR)/crti.o \
	    $(ARCHDIR)/crtbegin.o \
	    $(KERNEL_OBJS) \
	    $(LIBS) \
	    $(ARCHDIR)/crtend.o \
	    $(ARCHDIR)/crtn.o

# Build Rules
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -std=gnu11

.asm.o:
	$(AS) $(ASFLAGS) $< -o $@

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

# Functions
cocteauOS.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) $(CFLAGS) -T $(ARCHDIR)/linker.ld -o $@ $(LINK_LIST)
	grub-file --is-x86-multiboot cocteauOS.kernel

install-headers:
	mkdir -p $(INCLUDEDIR)
	cp -R include/. $(INCLUDEDIR)/.

install-kernel: cocteauOS.kernel
	mkdir -p $(BOOTDIR)
	cp cocteauOS.kernel $(BOOTDIR)

install: install-headers install-kernel


-include $(OBJS:.o=.d)

