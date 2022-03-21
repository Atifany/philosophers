# executable
PHILO = philo

# sources
_SRC = core.c utils.c
SRC_DIR = sources
SRC = $(_SRC:%=$(SRC_DIR)/%)

# headers
HEADS = philo.h

# libraries
#MLX = libmlx.a
#MLX_DIR = libs/mlx
#LIBFT = libft.a
#LIBFT_DIR = libs/libft
#FT_PRINTF = ft_printf.a
#FT_PRINTF_DIR = libs/ft_printf
#LIBS = $(MLX:%=$(MLX_DIR)/%) $(LIBFT:%=$(LIBFT_DIR)/%) $(FT_PRINTF:%=$(FT_PRINTF_DIR)/%)

# Make commands
GCC = gcc -O2 -Wall -Wextra -Werror
RM = rm -f

# rules
all: $(PHILO)

#compile_libs:
#	@make -C $(FT_PRINTF_DIR)
#	@make -C $(MLX_DIR)
#	@make -C $(LIBFT_DIR)

$(PHILO): $(SRC) $(HEADS)
	$(GCC) $(SRC) $(LIBS) -o $@

clean:
	$(RM) $(PHILO)

fclean: clean
#	@make clean -C $(MLX_DIR)
#	@make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all re clean compile_libs