FROM noctuelles/i386-gcc

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y build-essential nasm grub-pc-bin grub-common xorriso qemu-utils qemu-system-x86

VOLUME [ "/root/env" ]
WORKDIR /root/env

CMD ["/bin/bash"]