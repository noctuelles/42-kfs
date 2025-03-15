#!/bin/sh
set -e

TOOLSDIR=${TOOLSDIR:-"$(cd `dirname $0` && pwd)"}
PROJECTROOT=${PROJECTROOT:-"${TOOLSDIR}/.."}

. $TOOLSDIR/build.sh

cd $PROJECTROOT

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/$NAME isodir/boot/$NAME
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "$NAME" {
	multiboot /boot/$NAME
}

set timeout=1
EOF

grub-mkrescue -o $NAME.iso isodir