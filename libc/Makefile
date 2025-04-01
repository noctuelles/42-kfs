SRC_DIR=src
OBJ_DIR=obj
INC_DIR=include

CFLAGS?=-g
CPPFLAGS?=
LDFLAGS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Werror
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude
LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk

FREESTANDING_SRCS=string/strlen.c \
				  string/memset.c \
				  string/memmove.c \
				  string/memcpy.c \
				  string/memcmp.c \
				  string/strcmp.c \
				  string/itoa_base.c \
				  stdlib/abort.c \
				  stdlib/atoi.c \
				  stdio/putchar.c \
				  stdio/puts.c \
				  stdio/printf.c

FREESTANDING_OBJS=$(addprefix $(OBJ_DIR)/,$(FREESTANDING_SRCS:.c=.o))

LIBK_OBJS=$(FREESTANDING_OBJS:.o=.libk.o)

OBJS=\
$(FREESTANDING_OBJS)

BINARIES=libk.a

all: $(BINARIES)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BINARIES)

re: fclean all

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

$(OBJ_DIR)/%.libk.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.PHONY: all clean fclean re install install-headers install-libs