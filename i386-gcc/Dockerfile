FROM debian@sha256:70b337e820bf51d399fa5bfa96a0066fbf22f3aa2c3307e2401b91e2207ac3c3 AS builder

ENV TARGET=i386-elf \
    BINUTILS_VERSION=2.44 \
    GDB_VERSION=16.2 \
    GCC_VERSION=14.2.0 \
    NPROC=20

RUN apt update && apt upgrade -y
RUN apt install -y \
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
    libisl-dev

WORKDIR /tmp/cross

RUN wget https://mirror.easyname.at/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz && \
    tar -xf binutils-$BINUTILS_VERSION.tar.gz && \
    mkdir build-binutils-$BINUTILS_VERSION && \
    cd build-binutils-$BINUTILS_VERSION && \
    ../binutils-$BINUTILS_VERSION/configure --target=$TARGET --with-sysroot --disable-nls --disable-werror && \
    make -j $NPROC && \
    make install && \
    cd .. && \
    rm -rf binutils-$BINUTILS_VERSION binutils-$BINUTILS_VERSION.tar.gz build-binutils-$BINUTILS_VERSION

RUN wget https://mirror.easyname.at/gnu/gdb/gdb-$GDB_VERSION.tar.gz && \
    tar -xf gdb-$GDB_VERSION.tar.gz && \
    cd gdb-$GDB_VERSION && \
    ./configure --target=$TARGET --disable-werror --with-expat --enable-source-highlight --with-python && \
    make all-gdb -j $NPROC && \
    make install-gdb && \
    cd .. && \
    rm -rf gdb-$GDB_VERSION gdb-$GDB_VERSION.tar.gz

RUN wget https://mirror.easyname.at/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz && \
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
    rm -rf gcc-$GCC_VERSION gcc-$GCC_VERSION.tar.gz build-gcc-$GCC_VERSION

WORKDIR /