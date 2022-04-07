# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: atifany <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/18 17:50:48 by atifany           #+#    #+#              #
#    Updated: 2021/12/18 17:50:49 by atifany          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs

#general
NAME =			ft_printf.a
SRCDIR =		sources
#sources
DEPS =			ft_printf.h
SRC = 			ft_printf.c \
				ft_printf_utils_1.c ft_printf_utils_2.c\
				ft_printf_printers_1.c ft_printf_printers_2.c \
				ft_printf_printers_3.c
_SRC = $(SRC:%.c=$(SRCDIR)/%.c)
OBJ = $(_SRC:%.c=%.o)

#rules

all: $(NAME)

bonus: $(NAME)

$(NAME): $(OBJ)
						$(AR) $(NAME) $?

%.o: %.c $(DEPS)
						$(CC) $(CFLAGS) -o $@ -c $<

clean:
						rm -f $(OBJ)

fclean: clean
						rm -f $(NAME)

re: fclean all

.PHONY: re fclean clean all bonus
