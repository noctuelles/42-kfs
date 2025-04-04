#!/bin/bash

set -e

TOOLSDIR=${TOOLSDIR:-"$(cd `dirname $0` && pwd)"}
PROJECTROOT=${PROJECTROOT:-"${TOOLSDIR}/.."}

. $TOOLSDIR/config.sh

docker run --name $NAME -d -p 5901:5901 -v "$(pwd):/root/env" -it kfs/env \
    $QEMU -s -S -vnc :1 -cdrom $NAME.iso
sleep 0.5
vncviewer localhost:5901 &> /dev/null &
docker exec -it $NAME $GDB \
    -ex "file sysroot/boot/kfs" \
    -ex "target remote :1234" \
    -ex "directory kernel" \
    -ex "directory libc"
docker kill $NAME &> /dev/null
docker container remove $NAME &> /dev/null