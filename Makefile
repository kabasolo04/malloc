NAME        = libft_malloc

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

LIBNAME     = $(NAME)_$(HOSTTYPE).so
LINKNAME    = $(NAME).so

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -fPIC

SRC         = src/malloc.c
OBJ         = $(SRC:.c=.o)

all: $(LIBNAME)

$(LIBNAME): $(OBJ)
	$(CC) -shared $(OBJ) -o $(LIBNAME)
	ln -sf $(LIBNAME) $(LINKNAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(LIBNAME)
	rm -f $(LINKNAME)

re: fclean all

.PHONY: all clean fclean re