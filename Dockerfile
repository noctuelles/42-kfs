FROM noctuelles/gcc-cross-i386

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y build-essential nasm grub-pc-bin grub-common xorriso qemu-utils qemu-system-x86 && \
    rm -rf /var/lib/apt/lists/*

VOLUME [ "/root/env" ]
WORKDIR /root/env

CMD ["/bin/bash"]