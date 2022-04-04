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

char	_think(t_transfer *info)
{
	sem_wait(info->t_sems.sem_logs);
	if (!info->is_dead)
		printf("%s%lld: %d is thinking\n%s", CYN, cur_time(info->data), info->my_num + 1, NC);
	sem_post(info->t_sems.sem_logs);
	return (!info->is_dead);
}

char	_eat(t_transfer *info)
{
	sem_wait(info->t_sems.sem_forks);
	sem_wait(info->t_sems.sem_logs);
	if (!info->is_dead)
		printf("%s%lld: %d has taken a fork\n%s", CYN, cur_time(info->data), info->my_num + 1, NC);
	sem_post(info->t_sems.sem_logs);
	sem_wait(info->t_sems.sem_forks);
	pthread_mutex_lock(&info->eat_lock);
	sem_wait(info->t_sems.sem_logs);
	if (!info->is_dead)
	{
		printf("%s%lld: %d has taken a fork\n%s", CYN, cur_time(info->data), info->my_num + 1, NC);
		printf("%s%lld: %d is eating\n%s", CYN, cur_time(info->data), info->my_num + 1, NC);
	}
	info->t_philo.last_meal = cur_time(info->data);
	sem_post(info->t_sems.sem_logs);
	usleep(1000);
	while (cur_time(info->data) < info->t_philo.last_meal + info->data->time_to_eat)
	{
		if (info->is_dead)
			break ;
		usleep(1000);
	}
	pthread_mutex_unlock(&info->eat_lock);
	sem_post(info->t_sems.sem_forks);
	sem_post(info->t_sems.sem_forks);
	return (!info->is_dead);
}

char	_sleep(t_transfer *info)
{
	long long	timestamp;

	sem_wait(info->t_sems.sem_logs);
	if (!info->is_dead)
		printf("%s%lld: %d is sleeping\n%s", CYN, cur_time(info->data), info->my_num + 1, NC);
	sem_post(info->t_sems.sem_logs);
	timestamp = cur_time(info->data);
	usleep(1000);
	while (cur_time(info->data) < timestamp + (info->data->time_to_sleep))
	{
		if (info->is_dead)
			break ;
		usleep(1000);
	}
	return (!info->is_dead);
}
