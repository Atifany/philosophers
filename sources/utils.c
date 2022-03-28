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

void	unlock(pthread_mutex_t *lock_1,
	pthread_mutex_t *lock_2, pthread_mutex_t *lock_3)
{
	if (lock_1)
		pthread_mutex_unlock(lock_1);
	if (lock_2)
		pthread_mutex_unlock(lock_2);
	if (lock_3)
		pthread_mutex_unlock(lock_3);
}

long long	cur_time(t_data *data)
{
	struct timeval	te;
	long long		milliseconds;

	if (data)
		pthread_mutex_lock(&(data->get_time));
	gettimeofday(&te, NULL);
	milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
	if (data)
		pthread_mutex_unlock(&(data->get_time));
	if (data)
		return (milliseconds - data->sim_start);
	return (milliseconds);
}
