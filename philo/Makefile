# executable
PHILO = philo_bonus

# sources
_SRC = core.c utils.c think_eat_sleep.c inits.c
SRC_DIR = sources
SRC = $(_SRC:%=$(SRC_DIR)/%)
# tmp files
OBJ_DIR = tmp_files
OBJ = $(_SRC:%.c=$(OBJ_DIR)/%.o)
DPS = $(_SRC:%.c=$(OBJ_DIR)/%.d)

# headers
HEADS = philo_bonus.h

# libraries
_FT_PRINTF = ft_printf.a
FT_PRINTF_DIR = ../ft_printf
LIBS = $(_FT_PRINTF:%.a=$(FT_PRINTF_DIR)/%.a)

# Make commands
GCC = gcc
GCC_FLAGS = -O2 -Wall -Wextra -Werror
RM = rm -f

# rules
all: create_dirs compile_libs $(PHILO)

create_dirs:
	@mkdir -p tmp_files

compile_libs:
	@make bonus -C $(FT_PRINTF_DIR)

-include $(DPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(GCC) $(GCC_FLAGS) -c $< -o $@
	$(GCC) -MM $(GCC_FLAGS) $< > $(OBJ_DIR)/$*.d

$(PHILO): $(OBJ)
	$(GCC) $(OBJ) $(LIBS) -o $@

clean:
	$(RM) $(OBJ) $(DPS) $(PHILO)

fclean: clean
	@make fclean -C $(FT_PRINTF_DIR)

re: fclean all

.PHONY: all re clean compile_libs
