FROM techiekeith/gcc-cross-i686-elf

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y build-essential nasm grub-pc-bin grub-common xorriso

VOLUME [ "/root/env" ]
WORKDIR /root/env

CMD ["/bin/bash"]