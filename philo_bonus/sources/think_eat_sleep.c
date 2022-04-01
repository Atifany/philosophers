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

void	_think(t_data *data, int my_num, sem_t *sem_logs, sem_t *sem_gettime)
{
	sem_wait(sem_logs);
	printf("%s%lld: %d is thinking\n%s", CYN, cur_time(data, sem_gettime), my_num + 1, NC);
	sem_post(sem_logs);
}

void	_eat(t_data *data, int my_num, sem_t *sem_forks, sem_t *sem_logs, sem_t *sem_gettime)
{
	sem_wait(sem_forks);
	sem_wait(sem_logs);
	printf("%s%lld: %d has taken a fork\n%s", CYN, cur_time(data, sem_gettime), my_num + 1, NC);
	sem_post(sem_logs);
	sem_wait(sem_forks);
	sem_wait(sem_logs);
	printf("%s%lld: %d has taken a fork\n%s", CYN, cur_time(data, sem_gettime), my_num + 1, NC);
	printf("%s%lld: %d is eating\n%s", CYN, cur_time(data, sem_gettime), my_num + 1, NC);
	sem_post(sem_logs);
	usleep(data->time_to_eat * 1000);
	sem_post(sem_forks);
	sem_post(sem_forks);
}

void	_sleep(t_data *data, int my_num, sem_t *sem_logs, sem_t *sem_gettime)
{
	sem_wait(sem_logs);
	printf("%s%lld: %d is sleeping\n%s", CYN, cur_time(data, sem_gettime), my_num + 1, NC);
	sem_post(sem_logs);
	usleep(data->time_to_sleep * 1000);
}
