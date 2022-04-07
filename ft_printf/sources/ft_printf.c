/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 15:50:55 by atifany           #+#    #+#             */
/*   Updated: 2021/12/18 15:50:56 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static void	read_it(const char **format, t_formats *form, va_list ap)
{
	if (**format == '#' && ++(*format))
		form->is_hesh = 1;
	if (**format == '+' && ++(*format))
		form->is_plus = 1;
	if (**format == ' ' && ++(*format))
		form->is_space = 1;
	if (**format == '-' && ++(*format))
		form->is_minus = 1;
	if (**format == '0' && ++(*format))
		form->is_zero = 1;
	if ('1' <= **format && **format <= '9')
		form->width = read_number(format);
	if (**format == '.' && ++(*format))
	{
		form->is_dot = 1;
		form->precision = read_number(format);
	}
	if (**format == '*' && ++(*format))
	{
		if (form->is_dot == 1)
			form->precision = va_arg(ap, ULL);
		else
			form->width = va_arg(ap, ULL);
	}
}

static void	print_it(char ch, va_list ap, t_formats *form)
{
	if (ch == 'c')
		ft_putchar(va_arg(ap, unsigned int), form, 1);
	else if (ch == 's')
		ft_putstr(va_arg(ap, char *), form);
	else if (ch == 'p')
		ft_print_in_hex(va_arg(ap, ULL), 0, form, 1);
	else if (ch == 'd' || ch == 'i')
		ft_putnbr(va_arg(ap, int), form);
	else if (ch == 'u')
		ft_putnbr(va_arg(ap, unsigned int), form);
	else if (ch == 'x')
		ft_print_in_hex(va_arg(ap, unsigned int), 0, form, 0);
	else if (ch == 'X')
		ft_print_in_hex(va_arg(ap, unsigned int), 1, form, 0);
	else if (ch == '%')
		form->ret += write(1, "%", 1);
}

static char	check_if_format(const char *format)
{
	char	*alphabet;

	alphabet = "0123456789.-# +*";
	while (*alphabet)
		if (*(alphabet++) == *format)
			return (1);
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list		ap;
	t_formats	form;

	va_start(ap, format);
	clear_form(&form, 1);
	while (*format)
	{
		if (*format == '%')
		{
			++format;
			clear_form(&form, 0);
			while (check_if_format(format) == 1)
				read_it(&format, &form, ap);
			print_it(*format, ap, &form);
		}
		else
			form.ret += write(1, format, 1);
		++format;
	}
	va_end(ap);
	return (form.ret);
}
