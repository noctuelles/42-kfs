SRC_DIR=src
OBJ_DIR=obj
INC_DIR=include

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Werror
CPPFLAGS:=$(CPPFLAGS) -fno-builtin -fno-rtti -fno-exceptions -fno-stack-protector -D__is_kernel -Iinclude
LIBS:=$(LIBS) -nostdlib -lgcc -lk

KERNEL_CPP_SRCS=Kernel.cpp \
                io/VGATerminal.cpp

KERNEL_ASM_SRCS=Boot.S

KERNEL_C_RUNTIME_SRCS=crti.S \
					  crtn.S

KERNEL_CPP_OBJS=$(addprefix $(OBJ_DIR)/,$(KERNEL_CPP_SRCS:.cpp=.o))
KERNEL_ASM_OBJS=$(addprefix $(OBJ_DIR)/,$(KERNEL_ASM_SRCS:.S=.o))

KERNEL_OBJS=$(KERNEL_CPP_OBJS) $(KERNEL_ASM_OBJS)

OBJS=$(OBJ_DIR)/Crti.o \
$(OBJ_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(OBJ_DIR)/crtend.o \
$(OBJ_DIR)/Crtn.o

LINK_LIST=$(LDFLAGS) \
$(OBJ_DIR)/Crti.o \
$(OBJ_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(OBJ_DIR)/crtend.o \
$(OBJ_DIR)/Crtn.o

all: $(NAME)

$(NAME): $(OBJS) linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $(NAME) 

clean:
	rm -rf $(NAME)

fclean: clean
	rm -rf $(OBJ_DIR)

re: fclean all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MD -c $< -o $@ -std=c++11 $(CFLAGS) $(CPPFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

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