/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_printers_2_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 15:51:12 by atifany           #+#    #+#             */
/*   Updated: 2021/12/18 15:51:13 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	converter_to_base8(char *str, ULL n, char is_upper)
{
	int		i;
	char	*alphabet;
	char	*ualphabet;

	alphabet = "0123456789abcdef";
	ualphabet = "0123456789ABCDEF";
	i = 15;
	_ft_bzero(str, 17);
	if (n == 0)
		str[i--] = '0';
	while (n > 0)
	{
		if (is_upper)
			str[i--] = ualphabet[n % 16];
		else
			str[i--] = alphabet[n % 16];
		n /= 16;
	}
	return (++i);
}

static void	print_0x(t_formats *form, char is_ptr, ULL n, char is_upper)
{
	if (is_ptr == 1 || (form->is_hesh == 1 && n != 0))
	{
		if (is_upper)
			form->ret += write(1, "0X", 2);
		else
			form->ret += write(1, "0x", 2);
	}
}

static void	print_2(t_formats *form, size_t	ans_len, char *str, ULL n)
{
	if (form->is_zero == 1 && form->width > ans_len)
		print_width(form, ans_len);
	if (!(form->is_dot == 1 && n == 0 && form->precision == 0))
		form->ret += write(1, str, _ft_strlen(str));
	if (form->is_minus == 1 && form->width > ans_len)
		print_width(form, ans_len);
}

void	ft_print_in_hex(ULL n, char is_upper, t_formats *form, char is_ptr)
{
	char	str[17];
	int		i;
	size_t	ans_len;
	ULL		precision;

	i = converter_to_base8(str, n, is_upper);
	ans_len = _ft_strlen(&str[i]);
	precision = form->precision;
	if (form->is_dot == 1)
		form->is_zero = 0;
	if (n == 0 && form->is_dot == 1 && form->precision == 0)
		ans_len = 0;
	_ft_swap(form, &ans_len, &precision);
	if (is_ptr == 1 || (form->is_hesh == 1 && n != 0))
		ans_len += 2;
	if (form->is_minus == 0 && form->is_zero == 0)
		print_width(form, ans_len);
	print_0x(form, is_ptr, n, is_upper);
	while (form->is_dot && precision-- > 0)
		form->ret += write(1, "0", 1);
	print_2(form, ans_len, &str[i], n);
}
