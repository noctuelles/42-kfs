#!/bin/bash

set -e

TOOLSDIR=${TOOLSDIR:-"$(cd `dirname $0` && pwd)"}
PROJECTROOT=${PROJECTROOT:-"${TOOLSDIR}/.."}

. $TOOLSDIR/config.sh

docker run --name $NAME -d -p 5901:5901 -v "$(pwd):/root/env" -it kfs/env \
    $QEMU -vnc :1 -cdrom $NAME.iso
vncviewer localhost:5901 &> /dev/null
docker kill $NAME &> /dev/null
docker container remove $NAME &> /dev/null