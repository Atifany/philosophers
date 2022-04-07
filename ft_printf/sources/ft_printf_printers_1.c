/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_printers_1_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 15:51:05 by atifany           #+#    #+#             */
/*   Updated: 2021/12/18 15:51:07 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	ft_itoa(char *str, long long n)
{
	int	i;

	if (n < 0)
		n *= -1;
	_ft_bzero(str, 11);
	i = 9;
	if (n == 0)
		str[i--] = '0';
	while (n > 0)
	{
		str[i--] = n % 10 + '0';
		n /= 10;
	}
	return (++i);
}

static void	print_1(t_formats *form, char is_neg, size_t ans_len, ULL precision)
{
	if (form->is_minus == 0 && !(form->is_zero == 1 && is_neg == '-'))
		print_width(form, ans_len);
	if (is_neg == 0)
	{
		if (form->is_plus == 1)
			form->ret += write(1, "+", 1);
		else if (form->is_space == 1)
			form->ret += write(1, " ", 1);
	}
	else
		form->ret += write(1, "-", 1);
	while (form->is_dot && precision-- > 0)
		form->ret += write(1, "0", 1);
	if (form->is_minus == 0 && form->is_zero == 1 && is_neg == '-')
		print_width(form, ans_len);
}

static void	print_2(t_formats *form, size_t ans_len, int n, char *str)
{
	if (!(form->is_dot == 1 && n == 0 && form->precision == 0))
		form->ret += write(1, str, _ft_strlen(str));
	if (form->is_minus == 1)
		print_width(form, ans_len);
}

void	ft_putnbr(long long n, t_formats *form)
{
	char	str[11];
	int		i;
	char	is_neg;
	size_t	ans_len;
	ULL		precision;

	is_neg = 0;
	if (n < 0)
		is_neg = '-';
	i = ft_itoa(str, n);
	precision = form->precision;
	ans_len = _ft_strlen(&str[i]);
	if (form->is_dot == 1)
		form->is_zero = 0;
	if (n == 0 && form->is_dot == 1 && form->precision == 0)
		ans_len = 0;
	if (is_neg == '-' && form->width != 0)
		--form->width;
	if (form->width != 0 && (form->is_plus || form->is_space))
		--form->width;
	_ft_swap(form, &ans_len, &precision);
	print_1(form, is_neg, ans_len, precision);
	print_2(form, ans_len, n, &str[i]);
}
