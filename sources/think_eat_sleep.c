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
	if (check_if_someone_died(data))
		return (FALSE);
	printf("%s%lld: %d is thinking%s\n", CYN, cur_time(data), my_num, NC);
	return (TRUE);
}

char	_eat(t_data *data, int my_num, int left_fork, int right_fork)
{
	long long	timestamp;

	pthread_mutex_lock(&(data->forks[left_fork]));
	pthread_mutex_lock(&(data->forks[right_fork]));
	pthread_mutex_lock(&data->philo[my_num - 1].timer);
	if (check_if_someone_died(data))
	{
		pthread_mutex_unlock(&(data->forks[left_fork]));
		pthread_mutex_unlock(&(data->forks[right_fork]));
		pthread_mutex_unlock(&data->philo[my_num - 1].timer);
		return (FALSE);
	}
	if (data->philo[my_num - 1].feed_state[0] != 1)
		memset(data->philo[my_num - 1].feed_state, 2, 1);
	printf("%s%lld: %d is eating%s\n", CYN, cur_time(data), my_num, NC);
	timestamp = cur_time(data);
	usleep(1000);
	while (cur_time(data) < timestamp + (data->time_to_eat / 1000))
	{
		if (check_if_someone_died(data))
		{
			pthread_mutex_unlock(&(data->forks[left_fork]));
			pthread_mutex_unlock(&(data->forks[right_fork]));
			pthread_mutex_unlock(&data->philo[my_num - 1].timer);
			return (FALSE);
		}
		usleep(1000);
	}
	data->philo[my_num - 1].last_meal = cur_time(data);
	memset(data->philo[my_num - 1].feed_state, 0, 1);
	pthread_mutex_unlock(&data->philo[my_num - 1].timer);
	return (TRUE);
}

char	_sleep(t_data *data, int my_num, int left_fork, int right_fork)
{
	long long	timestamp;

	if (check_if_someone_died(data))
	{
		unlock(&(data->forks[left_fork]), &(data->forks[right_fork]), NULL);
		return (FALSE);
	}
	printf("%s%lld: %d is sleeping%s\n", CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&(data->forks[left_fork]));
	pthread_mutex_unlock(&(data->forks[right_fork]));
	timestamp = cur_time(data);
	usleep(1000);
	while (cur_time(data) < timestamp + (data->time_to_sleep / 1000))
	{
		if (check_if_someone_died(data))
			return (FALSE);
		usleep(1000);
	}
	return (TRUE);
}
