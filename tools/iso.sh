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
	boot
}
EOF

echo Generating ISO image...

grub-mkimage \
	--config="isodir/boot/grub/grub.cfg" \
	--output="/tmp/boot.img" \
	--prefix="(cd)" \
	--format=i386-pc \
	multiboot \
	biosdisk \
	iso9660

cat /usr/lib/grub/i386-pc/cdboot.img /tmp/boot.img > isodir/boot/grub/grub.img

genisoimage \
	-graft-points \
	-input-charset utf8 \
	-A "$NAME" \
	-quiet \
	-R \
	-b boot/grub/grub.img \
	-no-emul-boot \
	-boot-load-size 4 \
	-boot-info-table \
	-o $NAME.iso \
	isodir/