/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:27:54 by atifany           #+#    #+#             */
/*   Updated: 2022/03/21 15:27:57 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static ULL	ft_strlen(char *str)
{
	ULL	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

static ULL	ft_pow(int base, int pow)
{
	int	base_cpy;

	base_cpy = base;
	if (base <= 1)
		return (base);
	if (pow == 0)
		return (1);
	while (--pow)
		base *= base_cpy;
	return (base);
}

ULL	ft_atoi(char *n)
{
	int	ans;

	ans = 0;
	if (!n)
		return (ans);
	while (*n)
	{
		ans += (*n - '0') * ft_pow(10, ft_strlen(n) - 1);
		n++;
	}
	return (ans);
}