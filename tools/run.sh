#!/bin/bash

set -e

TOOLSDIR=${TOOLSDIR:-"$(cd `dirname $0` && pwd)"}
PROJECTROOT=${PROJECTROOT:-"${TOOLSDIR}/.."}

. $TOOLSDIR/config.sh

$DOCKER_RUN $QEMU -vnc :1 -m 512M -d cpu_reset -cdrom $NAME.iso
sleep 0.5
vncviewer localhost:5901 &> /dev/null
docker kill $NAME &> /dev/null
