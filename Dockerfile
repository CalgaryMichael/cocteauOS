# Builds an i686-elf cross compiler and builds kernel
FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y bison
RUN apt-get install -y curl
RUN apt-get install -y flex
RUN apt-get install -y gcc
RUN apt-get install -y g++
RUN apt-get install -y grub2
RUN apt-get install -y libgmp3-dev
RUN apt-get install -y libmpfr-dev
RUN apt-get install -y libmpc-dev
RUN apt-get install -y make
RUN apt-get install -y nasm
RUN apt-get install -y texinfo
RUN apt-get install -y xorriso

WORKDIR /opt/cross/src

ENV PREFIX=/opt/cross
ENV TARGET=i686-elf
ENV PATH=$PREFIX/bin:$PATH

RUN curl https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.gz -o binutils-2.37.tar.gz
RUN tar xvzf binutils-2.37.tar.gz -o binutils-2.37

RUN curl https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.gz -o gcc-11.2.0.tar.gz
RUN tar xvzf gcc-11.2.0.tar.gz -o gcc-11.2.0

# build custom binutils
RUN mkdir build-binutils && cd build-binutils; \
  ../binutils-2.37/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror; \
  make; \
  make install;

# build custom gcc
RUN mkdir build-gcc && cd build-gcc; \
  ../gcc-11.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers; \
  make all-gcc; \
  make all-target-libgcc; \
  make install-gcc; \
  make install-target-libgcc

# ensure that we use these cross compilers in the future
ENV CC=$TARGET-gcc
ENV AR=$TARGET-ar
ENV AS=nasm

# set up folders
WORKDIR /opt/cocteauOS
RUN mkdir -p libs build/root build/boot/grub

ENV BUILDDIR=/opt/cocteauOS/build
ENV ROOTDIR=/opt/cocteauOS/build/root
ENV BOOTDIR=/opt/cocteauOS/build/boot

COPY ./libs/ /opt/cocteauOS/libs/
COPY ./artifacts/* /tmp/artifacts/

# Build the kernel image
RUN cd libs && make install

# Convert the kernel image into an ISO image
RUN cp /tmp/artifacts/grub.cfg $BOOTDIR/grub/grub.cfg
RUN grub-mkrescue -o $BUILDDIR/cocteauOS.iso $BUILDDIR

