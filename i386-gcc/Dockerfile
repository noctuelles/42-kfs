FROM debian:bookworm-slim

ARG TARGET=i386-elf
ARG BINUTILS_VERSION=2.44
ARG GDB_VERSION=16.2
ARG GCC_VERSION=14.2.0
ARG NPROC

RUN apt update && apt upgrade -y && apt install -y \
    wget \
    build-essential \
    bison \
    flex \
    libgmp3-dev \
    libmpc-dev \
    libmpfr-dev \
    libexpat1-dev \
    texinfo \
    pkg-config \
    python3-dev \
    python-is-python3 \
    libsource-highlight-dev \
    libisl-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /tmp/cross

RUN wget https://mirror.easyname.at/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz && \
    tar -xf binutils-$BINUTILS_VERSION.tar.gz && \
    mkdir build-binutils-$BINUTILS_VERSION && \
    cd build-binutils-$BINUTILS_VERSION && \
    ../binutils-$BINUTILS_VERSION/configure --target=$TARGET --with-sysroot --disable-nls --disable-werror && \
    make -j $NPROC && \
    make install && \
    cd .. && \
    wget https://mirror.easyname.at/gnu/gdb/gdb-$GDB_VERSION.tar.gz && \
    tar -xf gdb-$GDB_VERSION.tar.gz && \
    cd gdb-$GDB_VERSION && \
    ./configure --target=$TARGET --disable-werror --with-expat --enable-source-highlight --with-python && \
    make all-gdb -j $NPROC && \
    make install-gdb && \
    cd .. && \
    wget https://mirror.easyname.at/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz && \
    tar -xf gcc-$GCC_VERSION.tar.gz && \
    mkdir build-gcc-$GCC_VERSION && \
    cd build-gcc-$GCC_VERSION && \
    ../gcc-$GCC_VERSION/configure --target=$TARGET --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx && \
    make all-gcc -j $NPROC && \
    make all-target-libgcc -j $NPROC && \
    make all-target-libstdc++-v3 -j $NPROC && \
    make install-gcc && \
    make install-target-libgcc && \
    make install-target-libstdc++-v3 && \
    cd .. && \
    rm -rf * 

WORKDIR /