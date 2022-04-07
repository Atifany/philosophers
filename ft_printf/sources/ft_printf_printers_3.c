/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_printers_3_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 15:51:18 by atifany           #+#    #+#             */
/*   Updated: 2021/12/18 15:51:19 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

void	ft_putchar(char ch, t_formats *form, char is_formated)
{
	if (is_formated == 1 && form->is_minus == 0)
		print_width(form, 1);
	form->ret += write(1, &ch, 1);
	if (is_formated == 1 && form->is_minus == 1)
		print_width(form, 1);
}

void	ft_putstr(char *str, t_formats *form)
{
	size_t	bytes_to_write;
	size_t	str_len;

	if (!str)
		str = "(null)";
	str_len = _ft_strlen(str);
	bytes_to_write = str_len;
	if (form->is_dot == 1 && bytes_to_write > form->precision)
		bytes_to_write = form->precision;
	if (form->is_minus == 0 && form->width > bytes_to_write)
		print_width(form, bytes_to_write);
	form->ret += write(1, str, bytes_to_write);
	if (form->is_minus == 1 && form->width > bytes_to_write)
		print_width(form, bytes_to_write);
}
