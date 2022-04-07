/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils_1_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 15:51:29 by atifany           #+#    #+#             */
/*   Updated: 2021/12/18 15:51:30 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

size_t	_ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (*(s++))
		i++;
	return (i);
}

void	_ft_bzero(void *s, size_t n)
{
	unsigned char	*b1;

	b1 = s;
	while (n-- > 0)
		*b1++ = 0;
}

static int	pow(int base, int power)
{
	int	base_save;

	base_save = base;
	if (power == 0)
		return (1);
	while (--power > 0)
		base *= base_save;
	return (base);
}

int	_ft_atoi(char *str)
{
	int		i;
	int		integer;
	int		ans_len;

	i = _ft_strlen(str);
	ans_len = 0;
	if (*str == '0')
		return (0);
	integer = 0;
	while (i-- > 0)
		integer += pow(10, i) * (str[ans_len++] - '0');
	return (integer);
}

void	_ft_swap(t_formats *form, size_t *ans_len, ULL *precision)
{
	size_t	temp;

	if (form->is_dot == 1)
	{
		if (form->precision > *ans_len)
		{
			temp = *ans_len;
			*ans_len = form->precision;
			*precision = *ans_len - temp;
		}
		else
			*precision = 0;
	}
}
