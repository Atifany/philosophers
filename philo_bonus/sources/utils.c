/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:42:16 by atifany           #+#    #+#             */
/*   Updated: 2022/04/01 14:42:17 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

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

long long	ft_atoi(char *str)
{
	long long	i;
	long long	integer;
	long long	ans_len;

	i = _ft_strlen(str);
	ans_len = 0;
	if (*str == '0')
		return (0);
	integer = 0;
	while (i-- > 0)
		integer += pow(10, i) * (str[ans_len++] - '0');
	return (integer);
}

long long	cur_time(t_data *data)
{
	struct timeval	te;
	long long		milliseconds;
	
	gettimeofday(&te, NULL);
	milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
	if (data)
		return (milliseconds - data->sim_start);
	return (milliseconds);
}
