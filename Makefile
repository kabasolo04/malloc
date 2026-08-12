CC          := cc

LIB_PATH    := libft_malloc
LIB         := $(LIB_PATH)/libft_malloc.so

CFLAGS      := -Wall -Wextra -Werror
CPPFLAGS    := -I$(LIB_PATH)/includes
LDFLAGS     := -L$(LIB_PATH)
LDLIBS      := -lft_malloc -pthread

FILE        ?= test.c
NAME        := test


.PHONY: all clean re show+ show-


show+:
	$(MAKE) -C $(LIB_PATH) show+


show-:
	$(MAKE) -C $(LIB_PATH) show-


all: $(NAME)


$(LIB):
	$(MAKE) -C $(LIB_PATH)


$(NAME): $(FILE) $(LIB)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(FILE) $(LDFLAGS) $(LDLIBS) -Wl,-rpath,$(LIB_PATH) -o $@


clean:
	rm -f $(NAME)
	$(MAKE) -C $(LIB_PATH) clean

fclean:
	rm -f $(NAME)
	$(MAKE) -C $(LIB_PATH) fclean

re: clean all