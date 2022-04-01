# executable
PHILO = philo

# sources
_SRC = core.c utils.c think_eat_sleep.c inits.c
SRC_DIR = sources
SRC = $(_SRC:%=$(SRC_DIR)/%)

# headers
HEADS = philo_bonus.h

# Make commands
GCC = gcc -O2 -Wall -Wextra -Werror
RM = rm -f

# rules
all: $(PHILO)

$(PHILO): $(SRC) $(HEADS)
	$(GCC) $(SRC) $(LIBS) -o $@

clean:
	$(RM) $(PHILO)

fclean: clean

re: fclean all

.PHONY: all re clean compile_libs