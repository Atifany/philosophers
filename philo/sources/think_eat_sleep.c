/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   think_eat_sleep.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:09:29 by atifany           #+#    #+#             */
/*   Updated: 2022/03/23 15:09:30 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	count_times_eaten(t_data *data, int my_num)
{
	pthread_mutex_lock(&(data->log_queue));
	data->philo[my_num - 1].times_eaten++;
	if (data->philo[my_num - 1].times_eaten
		== data->times_each_philosopher_must_eat)
		data->philososphers_hungry--;
	if (data->philososphers_hungry == 0)
		data->dead = TRUE;
	pthread_mutex_unlock(&(data->log_queue));
}

char	_think(t_data *data, int my_num)
{
	pthread_mutex_lock(&(data->log_queue));
	if (!data->dead)
		printf("%s%lld: %d is thinking%s\n", CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&(data->log_queue));
	if (data->dead)
		return (FALSE);
	return (TRUE);
}

static void	take_forks(t_data *data, int my_num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&(data->forks[left_fork]));
	pthread_mutex_lock(&(data->log_queue));
	if (!data->dead)
		printf("%s%lld: %d has taken left fork%s\n",
			CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&(data->log_queue));
	pthread_mutex_lock(&(data->forks[right_fork]));
	if (!data->dead)
		printf("%s%lld: %d has taken right fork%s\n",
			CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&(data->log_queue));
	pthread_mutex_lock(&(data->philo[my_num - 1].timer));
	pthread_mutex_lock(&data->log_queue);
	if (!data->dead)
		printf("%s%lld: %d is eating%s\n", CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&(data->log_queue));
}

char	_eat(t_data *data, int my_num, int left_fork, int right_fork)
{
	take_forks(data, my_num, left_fork, right_fork);
	data->philo[my_num - 1].last_meal = cur_time(data);
	usleep(1000);
	while (cur_time(data)
		< data->philo[my_num - 1].last_meal + (data->time_to_eat / 1000))
	{
		if (data->dead)
		{
			unlock(&(data->forks[left_fork]),
				&(data->forks[right_fork]), &(data->philo[my_num - 1].timer));
			return (FALSE);
		}
		usleep(1000);
	}
	count_times_eaten(data, my_num);
	return (TRUE);
}

char	_sleep(t_data *data, int my_num, int left_fork, int right_fork)
{
	long long	timestamp;

	pthread_mutex_lock(&data->log_queue);
	if (!data->dead)
		printf("%s%lld: %d is sleeping%s\n", CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&data->log_queue);
	unlock(&(data->forks[left_fork]),
		&(data->forks[right_fork]), &(data->philo[my_num - 1].timer));
	if (data->dead)
		return (FALSE);
	timestamp = cur_time(data);
	usleep(1000);
	while (cur_time(data) < timestamp + (data->time_to_sleep / 1000))
	{
		if (data->dead)
			return (FALSE);
		usleep(1000);
	}
	return (TRUE);
}
