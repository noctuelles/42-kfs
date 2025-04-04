SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel"

export MAKE=${MAKE:-make}
export HOST=i386-elf
export NAME=kfs

export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc
export CXX=${HOST}-g++
export GDB=${HOST}-gdb

export QEMU=qemu-system-i386
export DOCKER_IMAGE=noctuelles/kfs:latest
export DOCKER_RUN="docker run -d --rm --name $NAME -p 5901:5901 -v "$(pwd):/root/env" -it $DOCKER_IMAGE"

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export ISODIR=isodir

export CFLAGS='-O2 -g'
export CPPFLAGS=''

export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT"
export CXX="$CXX --sysroot=$SYSROOT"

if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
  export CC="$CC -isystem=$INCLUDEDIR"
  export CXX="$CXX -isystem=$INCLUDEDIR"
fi

