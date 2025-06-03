#!/bin/bash

set -e

TOOLSDIR=${TOOLSDIR:-"$(cd `dirname $0` && pwd)"}
PROJECTROOT=${PROJECTROOT:-"${TOOLSDIR}/.."}

. $TOOLSDIR/config.sh

$DOCKER_RUN $QEMU -vnc :1 -m 512M -cdrom $NAME.iso
vncviewer localhost:5901 &> /dev/null
docker kill $NAME &> /dev/null