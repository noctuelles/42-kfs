SRC_DIR=src
OBJ_DIR=obj
INC_DIR=include

CFLAGS?=-g
CPPFLAGS?=
LDFLAGS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Werror
CPPFLAGS:=$(CPPFLAGS) -fno-builtin -fno-stack-protector -D__is_kernel -Iinclude
LIBS:=$(LIBS) -nostdlib -lk -lgcc

KERNEL_CPP_SRCS=kernel.c \
                io/terminal.c \
                io/console/vga.c \
				io/keyboard/ps2_keyboard.c \
				io/keyboard/layout_qwerty.c

KERNEL_ASM_SRCS=boot.S

KERNEL_C_RUNTIME_SRCS=crti.S \
					  crtn.S

KERNEL_CPP_OBJS=$(addprefix $(OBJ_DIR)/,$(KERNEL_CPP_SRCS:.c=.o))
KERNEL_ASM_OBJS=$(addprefix $(OBJ_DIR)/,$(KERNEL_ASM_SRCS:.S=.o))

KERNEL_OBJS=$(KERNEL_CPP_OBJS) $(KERNEL_ASM_OBJS)

OBJS=$(OBJ_DIR)/crti.o \
$(OBJ_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(OBJ_DIR)/crtend.o \
$(OBJ_DIR)/crtn.o

LINK_LIST=$(LDFLAGS) \
$(OBJ_DIR)/crti.o \
$(OBJ_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(OBJ_DIR)/crtend.o \
$(OBJ_DIR)/crtn.o

all: $(NAME)

$(NAME): $(OBJS) linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $(NAME) 

clean:
	rm -rf $(NAME)

fclean: clean
	rm -rf $(OBJ_DIR)

re: fclean all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	$(AS) -msyntax=intel -mnaked-reg -c $< -o $@

$(OBJ_DIR)/crtbegin.o $(OBJ_DIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: $(NAME)
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $(NAME) $(DESTDIR)$(BOOTDIR)


.PHONY: all clean fclean re