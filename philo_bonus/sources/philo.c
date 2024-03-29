/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 18:08:59 by atifany           #+#    #+#             */
/*   Updated: 2022/04/16 18:09:00 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	_clock(t_transfer *info)
{
	long long	last_meal_save;
	long long	timestamp;

	while (TRUE)
	{
		last_meal_save = info->t_philo.last_meal;
		usleep(1000);
		while (cur_time(info->data)
			- info->t_philo.last_meal < info->data->time_to_die
			&& last_meal_save == info->t_philo.last_meal)
			usleep(1000);
		timestamp = cur_time(info->data);
		sem_wait(info->sem_eat);
		if (last_meal_save == info->t_philo.last_meal)
		{
			sem_wait(info->sem_log);
			ft_printf("%s%u: %d is dead\n%s",
				RED, timestamp, info->my_num + 1, NC);
			sem_post(info->sem_end);
			exit(0);
		}
		sem_post(info->sem_eat);
	}
}

void	*philo_life_cycle(void *arg)
{
	t_transfer	*info;
	long long	times_eaten;

	times_eaten = 0;
	info = (t_transfer *)arg;
	while (TRUE)
	{
		_eat(info);
		if (++times_eaten == info->data->times_each_philosopher_must_eat)
		{
			sem_wait(info->sem_log);
			ft_printf("%s%d: %d is full\n%s",
				YEL, cur_time(info->data), info->my_num + 1, NC);
			sem_post(info->philos_full);
			sem_post(info->sem_log);
		}
		_sleep(info);
	}
	return (NULL);
}

void	philosopher(t_transfer *info)
{
	pthread_t	tid;

	pthread_create(&tid, NULL, philo_life_cycle, info);
	_clock(info);
}
