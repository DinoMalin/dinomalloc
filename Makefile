NAME = malloc

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
LIB = libft_malloc_$(HOSTTYPE).so

LIBFT = lib/libft.a

CC = cc

CFLAGS =	-Wall -Wextra -Werror -O3 -g \
			-Iinclude -Ilib/include

LFLAGS =	-Llib -lft

SRC_DIR = src/
OBJ_DIR = obj/
FILES =	malloc free realloc utils valgrind

SRCS = $(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))
OBJS = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(FILES)))

all: $(LIB)
test: $(NAME)

clean :
	@rm -rf $(OBJ_DIR)

fclean : clean
	@rm -rf $(LIB)
	@make fclean -C lib

re: fclean all

$(LIB): CFLAGS += -fPIC
$(LIB): $(OBJS) $(LIBFT)
	@echo "\e[32m✔ Linking sources files...\e[37m"
	@$(CC) -shared -o $@ $(OBJS) $(LFLAGS)
	@echo "\e[32m✔ Shared object created.\e[37m"


$(NAME): FILES += main
$(NAME): $(OBJS) obj/main.o $(LIBFT)
	@echo "\e[32m✔ Linking sources files...\e[37m"
	@$(CC) -o $@ $(OBJS)
	@echo "\e[32m✔ Executable created.\e[37m"

$(LIBFT):
	@make -C lib

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re bonus
