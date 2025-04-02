#!/bin/sh
set -e

TOOLSDIR=${TOOLSDIR:-"$(cd `dirname $0` && pwd)"}
PROJECTROOT=${PROJECTROOT:-"${TOOLSDIR}/.."}

. $TOOLSDIR/config.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECTROOT/$PROJECT && DESTDIR="$SYSROOT" $MAKE fclean)
done

rm -rf $SYSROOT
rm -rf $ISODIR
rm -rf $NAME.iso