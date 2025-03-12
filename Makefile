SRCS_DIR = srcs
OBJS_DIR = objs
ISO_DIR  = iso

DOCKER_IMAGE = plouvel/kfs-buildenv
DOCKER_RUN   = docker run --rm -it -v "$(PWD):/root/env" $(DOCKER_IMAGE) 

CC     = $(DOCKER_RUN) i686-elf-gcc
AS     = $(DOCKER_RUN) i686-elf-as
LINK   = $(DOCKER_RUN) i686-elf-gcc -T linker.ld
GRUB   = $(DOCKER_RUN) grub-mkrescue /usr/lib/grub/i386-pc
RM     = rm -rf

NAME     = kfs
GRUB_CFG = grub.cfg

CFLAGS = -fno-builtin \
		 -fno-stack-protector \
		 -nostdlib \
		 -nodefaultlibs \
		 -std=gnu99 \
		 -ffreestanding

CLIBS  = -lgcc

C_SRCS   = kernel.c
ASM_SRCS = boot.s

C_OBJS = $(addprefix $(OBJS_DIR)/, $(C_SRCS:.c=.o))
ASM_OBJS = $(addprefix $(OBJS_DIR)/, $(ASM_SRCS:.s=.o))

all: $(NAME).iso

$(NAME).iso: $(C_OBJS) $(ASM_OBJS) $(GRUB_CFG)
	$(LINK) $(CFLAGS) -o $(OBJS_DIR)/$(NAME).bin $(C_OBJS) $(ASM_OBJS) $(CLIBS)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(OBJS_DIR)/$(NAME).bin $(ISO_DIR)/boot/grub
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub
	$(GRUB) -o $(NAME).iso $(ISO_DIR)
	$(RM) $(ISO_DIR)

boot: $(NAME).iso
	qemu-system-i386 -cdrom $(NAME).iso

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME).bin
	$(RM) $(NAME).iso

.PHONY: clean fclean all boot