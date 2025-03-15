NAME?=kfs
TOOLS_DIR=tools

#docker build -t kfs/build-env .
#docker run -v "$(pwd):/root/env" -it kfs/build-env make iso

build:
	@bash -c "$(TOOLS_DIR)/build.sh"
iso:
	@bash -c "$(TOOLS_DIR)/iso.sh"
clean:
	@bash -c "$(TOOLS_DIR)/clean.sh"
launch:
	qemu-system-i386 -cdrom $(NAME).iso
launch-dbg:
	qemu-system-i386 -m 512M -s -S -cdrom $(NAME).iso

.PHONY: build iso clean launch launch-dbg