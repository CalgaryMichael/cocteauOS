.PHONY: all clean install-headers install-kernel
.SUFFIXES: .o .c .asm

DESTDIR ?=
CCFLAGS = -m32 -march=i386

LIBS := $(LIBS) -nostdlib

OBJS = \
       boot.o \
       kernel.o

# TODO: still need to add the "crt..." files
LINK_LIST = \
	    $(OBJS) \
	    $(LIBS)

# Build Rules
.c.o:
	gcc $(CCFLAGS) -c $< -o $@ -std=gnu11 -ffreestanding -Wall -Wextra

.asm.o:
	nasm -f elf32 $< -o $@

cocteauOS.kernel: $(OBJS) linker.ld
	gcc $(CCFLAGS) -T linker.ld -o $@ $(LINK_LIST)
	grub-file --is-x86-multiboot cocteauOS.kernel

install: cocteauOS.kernel
	mkdir -p $(DESTDIR)
	cp cocteauOS.kernel $(DESTDIR)

