CC = cc
PRINT_ALL_MEM ?= 0
CFLAGS = -Wall -Wextra -Werror -fPIC -Iincludes -Ilibft \
			-DPRINT_ALL_MEM=$(PRINT_ALL_MEM)
LDFLAGS = -shared

LIBFT_DIR = libft

NAME = libft_malloc
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
LIBNAME = $(NAME)_$(HOSTTYPE).so
LINKNAME = $(NAME).so
TESTNAME = test

SRCS = $(wildcard srcs/*.c)
OBJDIR = objs
OBJS = $(patsubst srcs/%.c,$(OBJDIR)/%.o,$(SRCS))
LIBFT_LIB = $(LIBFT_DIR)/libft.a

all: $(LIBNAME) $(LINKNAME)

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: srcs/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBNAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBFT_LIB) -o $(LIBNAME)

$(LINKNAME): $(LIBNAME)
	ln -sf $(LIBNAME) $(LINKNAME)

test: $(LIBNAME) test.c
	$(CC) test.c -pthread -L. -Wl,-rpath,. -lft_malloc -o $(TESTNAME)

show+:
	$(MAKE) re PRINT_ALL_MEM=1

show-:
	$(MAKE) re PRINT_ALL_MEM=0

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(LIBNAME) $(LINKNAME) $(TESTNAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all test

.PHONY: all clean fclean re test show+ show-
