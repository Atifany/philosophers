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

char	_eat(t_data *data, int my_num, int left_fork, int right_fork)
{
	long long	timestamp;

	//left fork
	pthread_mutex_lock(&(data->forks[left_fork]));
	pthread_mutex_lock(&(data->log_queue));
	if (!data->dead)
		printf("%s%lld: %d has taken left fork%s\n", CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&(data->log_queue));
	//right fork
	pthread_mutex_lock(&(data->forks[right_fork]));
	if (!data->dead)
		printf("%s%lld: %d has taken right fork%s\n", CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&(data->log_queue));
	pthread_mutex_lock(&(data->philo[my_num - 1].timer));
	pthread_mutex_lock(&data->log_queue);
	if (!data->dead)
		printf("%s%lld: %d is eating%s\n", CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&(data->log_queue));
	if (data->dead)
	{
		unlock(&(data->forks[left_fork]),
			   &(data->forks[right_fork]), &(data->philo[my_num - 1].timer));
		return (FALSE);
	}
	//eating
	timestamp = cur_time(data);
	usleep(1000);
	while (cur_time(data) < timestamp + (data->time_to_eat / 1000))
	{
		if (data->dead)
		{
			unlock(&(data->forks[left_fork]),
				   &(data->forks[right_fork]), &(data->philo[my_num - 1].timer));
			return (FALSE);
		}
		usleep(1000);
	}
	data->philo[my_num - 1].last_meal = cur_time(data);
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
