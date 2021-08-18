FROM gcc:11.2.0

# install nasm
RUN apt-get update && apt-get install -y nasm grub2 gcc-multilib g++-multilib

# set up folders
WORKDIR /opt/cocteauOS
RUN mkdir libs build

ENV DESTDIR=/opt/cocteauOS/build

COPY ./libs/* /opt/cocteauOS/libs/

RUN cd libs && make install

