SRCS_DIR = srcs
OBJS_DIR = objs

CC     = i686-elf-gcc
AS     = i686-elf-as
LINK   = i686-elf-gcc -T linker.ld
RM     = rm -rf

NAME   = kfs

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

kfs: $(C_OBJS) $(ASM_OBJS)
	$(LINK) $(CFLAGS) -o $(NAME).bin $^ $(CLIBS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR)

