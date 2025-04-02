#!/bin/sh
set -e

TOOLSDIR=${TOOLSDIR:-"$(cd `dirname $0` && pwd)"}
PROJECTROOT=${PROJECTROOT:-"${TOOLSDIR}/.."}

. $TOOLSDIR/build.sh

cd $PROJECTROOT

mkdir -p $ISODIR
mkdir -p $ISODIR/boot
mkdir -p $ISODIR/boot/grub

cp sysroot/boot/$NAME $ISODIR/boot/$NAME
cat > $ISODIR/boot/grub/grub.cfg << EOF
menuentry "$NAME" {
	multiboot /boot/$NAME
	boot
}
EOF

set -x

grub-mkimage \
	--config="$ISODIR/boot/grub/grub.cfg" \
	--output="/tmp/core.img" \
	--prefix="(cd)" \
	--format=i386-pc \
	multiboot \
	biosdisk \
	iso9660

# Copy the stage 1 bootloader of GRUB 2
dd if=/usr/lib/grub/i386-pc/cdboot.img of=$ISODIR/boot/grub/grub.img
# Append the stage 1.5 generated GRUB 2 core image
dd if=/tmp/core.img of=$ISODIR/boot/grub/grub.img conv=notrunc oflag=append

rm -rf /tmp/core.img

# Generate an El Torito image.
xorriso -as mkisofs \
	-graft-points \
	-input-charset utf8 \
	-A "$NAME" \
	-R \
	-b boot/grub/grub.img \
	-no-emul-boot \
	-boot-load-size 4 \
	-boot-info-table \
	-o $NAME.iso \
	$ISODIR/