/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   think_eat_sleep.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:42:09 by atifany           #+#    #+#             */
/*   Updated: 2022/04/01 14:42:10 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

static char	put_log(t_transfer *info, char *log_to_print)
{
	//ft_printf("$%d\n", info->my_num + 1);
	sem_wait(info->dead_state);
	//ft_printf("|%d|%d|\n", info->my_num + 1, info->is_dead);
	if (info->is_dead)
	{
		sem_post(info->dead_state);
		return (FALSE);
	}
	sem_post(info->dead_state);
	sem_wait(info->sem_logs);
	ft_printf("%s%d: %d %s\n%s",
		CYN, cur_time(info->data), info->my_num + 1, log_to_print, NC);
	sem_post(info->sem_logs);
	return (TRUE);
}

char	_eat(t_transfer *info)
{
	if (!put_log(info, "is thinking"))
		return (FALSE);
	sem_wait(info->sem_forks);
	if (!put_log(info, "has taken a fork"))
	{
		sem_post(info->sem_forks);
		return (FALSE);
	}
	sem_wait(info->sem_forks);
	if (!put_log(info, "has taken a fork"))
	{
		sem_post(info->sem_forks);
		sem_post(info->sem_forks);
		return (FALSE);
	}
	sem_wait(info->eating_state);
	if (!put_log(info, "is eating"))
	{
		sem_post(info->sem_forks);
		sem_post(info->sem_forks);
		sem_post(info->eating_state);
		return (FALSE);
	}
	info->t_philo.last_meal = cur_time(info->data);
	sem_post(info->eating_state);
	usleep(1000);
	while (!info->is_dead && cur_time(info->data)
		< info->t_philo.last_meal + info->data->time_to_eat)
		usleep(1000);
	sem_post(info->sem_forks);
	sem_post(info->sem_forks);
	return (TRUE);
}

char	_sleep(t_transfer *info)
{
	long long	timestamp;

	if (!put_log(info, "is sleeping"))
		return (FALSE);
	timestamp = cur_time(info->data);
	usleep(1000);
	while (!info->is_dead && cur_time(info->data) < timestamp + (info->data->time_to_sleep))
		usleep(1000);
	return (TRUE);
}
