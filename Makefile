NAME?=kfs
TOOLS_DIR=tools
DOCKER_TAG=noctuelles/kfs
DOCKER_CMD=docker run -v "$(shell pwd):/root/env" -it $(DOCKER_TAG)

build:
	 @$(DOCKER_CMD) $(TOOLS_DIR)/build.sh

$(NAME).iso: build
	 @$(DOCKER_CMD) $(TOOLS_DIR)/iso.sh

run: $(NAME).iso
	 @bash -c -i "$(TOOLS_DIR)/run.sh"

run-dbg: $(NAME).iso
	 @bash -c -i "$(TOOLS_DIR)/run-dbg.sh"

clean:
	 @$(DOCKER_CMD) $(TOOLS_DIR)/clean.sh

.PHONY: image build run run-dbg clean