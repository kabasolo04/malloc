CC              := cc

PRINT_ALL_MEM   ?= 0

CFLAGS          := -Wall -Wextra -Werror -fPIC -Iincludes -Ilibft
CPPFLAGS        := -DPRINT_ALL_MEM=$(PRINT_ALL_MEM)
LDFLAGS         := -shared

NAME            := libft_malloc
HOSTTYPE        := $(shell uname -m)_$(shell uname -s)
LIBNAME         := $(NAME)_$(HOSTTYPE).so
LINKNAME        := $(NAME).so

LIBFT_DIR       := libft
LIBFT_LIB       := $(LIBFT_DIR)/libft.a

SRC_DIR         := malloc/srcs
OBJ_DIR         := malloc/objs

SRCS            := $(wildcard $(SRC_DIR)/*.c)
OBJS            := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


.PHONY: all clean fclean re show+ show-


all: $(LIBNAME) $(LINKNAME)


$(LIBNAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBFT_LIB) -o $@


$(LINKNAME): $(LIBNAME)
	ln -sf $< $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@


$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)


show+:
	$(MAKE) re PRINT_ALL_MEM=1


show-:
	$(MAKE) re PRINT_ALL_MEM=0


clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean


fclean: clean
	rm -f $(LIBNAME) $(LINKNAME)
	$(MAKE) -C $(LIBFT_DIR) fclean


re: fclean all