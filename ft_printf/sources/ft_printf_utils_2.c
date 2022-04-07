/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils_2_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 15:51:34 by atifany           #+#    #+#             */
/*   Updated: 2021/12/18 15:51:36 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

void	clear_form(t_formats *form, char is_first_time)
{
	if (is_first_time == 1)
		form->ret = 0;
	form->is_hesh = 0;
	form->is_plus = 0;
	form->is_space = 0;
	form->width = 0;
	form->precision = 0;
	form->is_dot = 0;
	form->is_minus = 0;
	form->is_zero = 0;
}

int	read_number(const char **format)
{
	char	digit_read[11];
	int		i;

	_ft_bzero(digit_read, 11);
	i = 0;
	while ('0' <= **format && **format <= '9')
	{
		digit_read[i++] = **format;
		++(*format);
	}
	return (_ft_atoi(digit_read));
}

void	print_width(t_formats *form, size_t ans_len)
{
	if (form->width > ans_len)
	{
		if (form->is_zero == 1)
			while (form->width > ans_len++)
				form->ret += write(1, "0", 1);
		else
			while (form->width > ans_len++)
				form->ret += write(1, " ", 1);
	}
}
