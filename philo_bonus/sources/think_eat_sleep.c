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

static void	put_log(t_transfer *info, char *log_to_print)
{
	sem_wait(info->sem_logs);
	ft_printf("%s%d: %d %s\n%s",
		CYN, cur_time(info->data), info->my_num + 1, log_to_print, NC);
	sem_post(info->sem_logs);
}

void	_eat(t_transfer *info)
{
	put_log(info, "is thinking");
	sem_wait(info->sem_forks);
	put_log(info, "has taken a fork");
	sem_wait(info->sem_forks);
	put_log(info, "has taken a fork");
	sem_wait(info->eating_state);
	put_log(info, "is eating");
	info->t_philo.last_meal = cur_time(info->data);
	sem_post(info->eating_state);
	usleep(1000);
	while (cur_time(info->data)
		< info->t_philo.last_meal + info->data->time_to_eat)
		usleep(1000);
	sem_post(info->sem_forks);
	sem_post(info->sem_forks);
}

void	_sleep(t_transfer *info)
{
	long long	timestamp;

	put_log(info, "is sleeping");
	timestamp = cur_time(info->data);
	usleep(1000);
	while (cur_time(info->data) < timestamp + (info->data->time_to_sleep))
		usleep(1000);
}
