DESTDIR ?=
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot
INCLUDE_DIR ?= $(PREFIX)/include
LIBDIR ?= $(EXEC_PREFIX)/lib

# Target architecture
HOST ?= $(shell ../../bin/default-host)
HOST_ARCH ?= $(shell ../../bin/target-triplet-to-arch $(HOST))
ARCHDIR = arch/$(HOST_ARCH)
 
include $(ARCHDIR)/make.config

# Flags
CFLAGS ?= -O2 -g
CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra $(ARCH_CFLAGS)
CPPFLAGS := $(CPPFLAGS) -D__is_libc -Iinclude $(ARCH_CPPFLAGS)
LIBK_CFLAGS := $(CFLAGS) $(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS := $(CPPFLAGS) $(LIBK_CPPFLAGS) -D__is_libk $(KERNEL_ARCH_CPPFLAGS)
 
FREEOBJS=\
$(ARCH_FREEOBJS) \
stdio/printf.o \
stdio/putchar.o \
stdio/puts.o \
stdlib/abort.o \
string/memcmp.o \
string/memcpy.o \
string/memmove.o \
string/memset.o \
string/strlen.o \
 
HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \
 
OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \
 
LIBK_OBJS=$(FREEOBJS:.o=.libk.o)
 
#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=libk.a
 
.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c
 
 
# Build Rules
libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
 
libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)
 
.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
 
.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)


# Functions
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDE_DIR)
	cp -R include/. $(DESTDIR)$(INCLUDE_DIR)/.
 
install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)


# Commands
all: $(BINARIES)
install: install-headers install-libs
clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

 
-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)

