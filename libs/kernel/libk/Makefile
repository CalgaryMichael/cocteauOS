HOST ?= i386
ARCHDIR = arch/$(HOST)

include $(ARCHDIR)/make.config

.PHONY: install-headers install-libs
.SUFFIXES: .o .c .asm

ROOTDIR ?=
BOOTDIR ?=
PREFIX ?= $(ROOTDIR)/usr
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR = $(PREFIX)/lib

# Flags
CFLAGS = --sysroot=$(ROOTDIR) -isystem=$(INCLUDEDIR) -I$(INCLUDEDIR) -O2 -g -ffreestanding -Wall -Wextra $(ARCH_CFLAGS)
CPPFLAGS = $(ARCH_CPPFLAGS)

OBJS=\
     $(ARCH_OBJS) \
     stdio/printf.o \
     stdio/putchar.o \
     stdio/puts.o \
     stdlib/abort.o \
     string/memcmp.o \
     string/memcpy.o \
     string/memmove.o \
     string/memset.o \
     string/strlen.o

# Build rules
libk.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -MD -c $< -o $@ -std=gnu11

# Functions
install-headers:
	mkdir -p $(INCLUDEDIR)
	cp -R include/. $(INCLUDEDIR)/.

install-libs: libk.a
	mkdir -p $(LIBDIR)
	cp libk.a $(LIBDIR)

install: install-headers install-libs

-include $(OBJS:.o=.d)

