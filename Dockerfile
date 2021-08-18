FROM gcc:11.2.0

# install nasm
RUN apt-get update && apt-get install -y nasm grub2 xorriso

# set up folders
WORKDIR /opt/cocteauOS
RUN mkdir -p libs build/boot/grub

ENV BUILDDIR=/opt/cocteauOS/build
ENV DESTDIR=/opt/cocteauOS/build/boot

COPY ./libs/* /opt/cocteauOS/libs/
COPY ./artifacts/* /tmp/artifacts/

# Build the kernel image
RUN cd libs && make install

# Convert the kernel image into an ISO image
RUN cp /tmp/artifacts/grub.cfg $DESTDIR/grub/grub.cfg
RUN grub-mkrescue -o $BUILDDIR/cocteauOS.iso $BUILDDIR

