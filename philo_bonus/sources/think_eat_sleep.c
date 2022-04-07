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

void	_think(t_transfer *info)
{
	sem_wait(info->sem_logs);
	if (!info->is_dead)
		ft_printf("%s%d: %d is thinking\n%s", CYN, cur_time(info->data), info->my_num + 1, NC);
	sem_post(info->sem_logs);
	return ;
}

void	_eat(t_transfer *info)
{
	sem_wait(info->sem_forks);
	sem_wait(info->sem_logs);
	if (!info->is_dead)
		ft_printf("%s%d: %d has taken a fork\n%s", CYN, cur_time(info->data), info->my_num + 1, NC);
	sem_post(info->sem_logs);
	sem_wait(info->sem_forks);
	pthread_mutex_lock(&info->eat_lock);
	sem_wait(info->sem_logs);
	if (!info->is_dead)
	{
		ft_printf("%s%d: %d has taken a fork\n%s", CYN, cur_time(info->data), info->my_num + 1, NC);
		ft_printf("%s%d: %d is eating\n%s", CYN, cur_time(info->data), info->my_num + 1, NC);
	}
	info->t_philo.last_meal = cur_time(info->data);
	sem_post(info->sem_logs);
	usleep(1000);
	while (cur_time(info->data) < info->t_philo.last_meal + info->data->time_to_eat)
		usleep(1000);
	pthread_mutex_unlock(&info->eat_lock);
	sem_post(info->sem_forks);
	sem_post(info->sem_forks);
	return ;
}

void	_sleep(t_transfer *info)
{
	long long	timestamp;

	sem_wait(info->sem_logs);
	if (!info->is_dead)
		ft_printf("%s%d: %d is sleeping\n%s", CYN, cur_time(info->data), info->my_num + 1, NC);
	sem_post(info->sem_logs);
	timestamp = cur_time(info->data);
	usleep(1000);
	while (cur_time(info->data) < timestamp + (info->data->time_to_sleep))
		usleep(1000);
	return ;
}
