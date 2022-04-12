# executable
PHILO = philo_bonus

# sources
_SRC = core.c utils_1.c utils_2.c think_eat_sleep.c inits.c
SRC_DIR = sources
SRC = $(_SRC:%=$(SRC_DIR)/%)

# headers
HEADS = philo_bonus.h

# libraries
_FT_PRINTF = ft_printf.a
FT_PRINTF_DIR = ../ft_printf
LIBS = $(_FT_PRINTF:%.a=$(FT_PRINTF_DIR)/%.a)

# Make commands
GCC = gcc -O2 -Wall -Wextra -Werror
RM = rm -f

# rules
all: compile_libs $(PHILO)

compile_libs:
	@make bonus -C $(FT_PRINTF_DIR)

$(PHILO): $(SRC) $(HEADS)
	$(GCC) $(SRC) $(LIBS) -o $@

clean:
	$(RM) $(PHILO)

fclean: clean
	@make fclean -C $(FT_PRINTF_DIR)

re: fclean all

.PHONY: all re clean compile_libs