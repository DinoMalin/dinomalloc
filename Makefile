NAME = malloc

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
LIB = libft_malloc_$(HOSTTYPE).so

LIBFT = lib/libft.a

CC = cc

CFLAGS =	-Wall -Wextra -Werror -O3 -g \
			-Iinclude -Ilib/include

LFLAGS =	-Llib -lft -Iinclude -Ilib/include
LFLAGS_TEST = -L. -lft_malloc_$(HOSTTYPE) -Iinclude -Ilib/include

SRC_DIR = src/
OBJ_DIR = obj/
FILES =	malloc free realloc utils valgrind

SRCS = $(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))
OBJS = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(FILES)))

all: $(LIB)
test: $(LIB) $(NAME)

clean :
	@rm -rf $(OBJ_DIR)

fclean : clean
	@rm -rf $(LIB) $(NAME)
	@make fclean -C lib --silent

re: fclean all

$(LIB): CFLAGS += -fPIC
$(LIB): $(OBJS) $(LIBFT)
	@echo "\e[32m✔ Linking source files...\e[37m"
	@$(CC) -shared -o $@ $(OBJS) $(LFLAGS)
	@ln -sf $(LIB) libft_malloc.so
	@echo "\e[32m✔ Shared object created.\e[37m"


$(NAME): FILES += main
$(NAME): $(OBJS) obj/main.o $(LIBFT)
	@echo "\e[32m✔ Linking shared object...\e[37m"
	@$(CC) $(CFLAGS) -o $@ src/main.c $(LFLAGS_TEST)
	@echo "\e[32m✔ Executable created.\e[37m"
	@echo "don't forget to add the current directory to your ld library path:"
	@echo "export LD_LIBRARY_PATH="$(CURDIR)':$$LD_LIBRARY_PATH'

$(LIBFT):
	@make -C lib --silent

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re bonus
