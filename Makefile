CC          := cc

LIB_PATH    := libft_malloc
TEST_PATH   := tests
LIB         := $(LIB_PATH)/libft_malloc.so

CFLAGS      := -Wall -Wextra -Werror
INCLUDES    := -I$(LIB_PATH)/includes
LDFLAGS     := -L$(LIB_PATH)
LDLIBS      := -lft_malloc -pthread

TEST        ?= 00
NAME        := test

.PHONY: all clean fclean re show+ show- real

all: $(NAME)

$(LIB):
	$(MAKE) -C $(LIB_PATH)

$(NAME): $(TEST_PATH)/test_$(TEST).c $(LIB)
	$(CC) $(CFLAGS) $(INCLUDES) $< \
		$(LDFLAGS) $(LDLIBS) \
		-Wl,-rpath,$(LIB_PATH) \
		-o $@

show+:
	$(MAKE) -C $(LIB_PATH) show+

show-:
	$(MAKE) -C $(LIB_PATH) show-

real:
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(TEST_PATH)/test_$(TEST).c \
		-o $(NAME)

clean:
	rm -f $(NAME)
	$(MAKE) -C $(LIB_PATH) clean

fclean:
	rm -f $(NAME)
	$(MAKE) -C $(LIB_PATH) fclean

re: fclean all