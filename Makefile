NAME?=kfs
TOOLS_DIR=tools
DOCKER_TAG=kfs/env
DOCKER_CMD=docker run -p 5901:5901 -v "$(shell pwd):/root/env" -it $(DOCKER_TAG)

image:
	docker build -t $(DOCKER_TAG) .

build: image
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