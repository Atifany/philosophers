/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:41:58 by atifany           #+#    #+#             */
/*   Updated: 2022/04/01 14:41:58 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	*_clock(void *arg)
{
	t_transfer	*info;
	long long	last_meal_save;
	long long	timestamp;

	info = (t_transfer *)arg;
	last_meal_save = info->t_philo.last_meal;
	usleep(1000);
	while (!info->is_dead
		&& cur_time(info->data) - info->t_philo.last_meal < info->data->time_to_die
		&& last_meal_save == info->t_philo.last_meal)
		usleep(1000);
	timestamp = cur_time(info->data);
	//replace mutex with semaphore!!!!!!
	pthread_mutex_lock(&info->eat_lock);
	if (last_meal_save == info->t_philo.last_meal)
	{
		sem_wait(info->t_sems.sem_logs);
		if (!info->is_dead)
			printf("%s%llu: %d is dead\n%s", RED, timestamp, info->my_num + 1, NC);
		info->is_dead = TRUE;
		sem_post(info->t_sems.sem_forks);
		sem_post(info->t_sems.sem_forks);
	}
	pthread_mutex_unlock(&info->eat_lock);
	return (NULL);
}

void	philosopher(t_transfer *info)
{
	pthread_t	tid;

	info->is_dead = FALSE;
	pthread_mutex_init(&(info->eat_lock), NULL);
	pthread_create(&tid, NULL, _clock, info);
	while (TRUE)
	{
		if (!_think(info))
			break ;
		if (!_eat(info))
			break ;
		if (!_sleep(info))
			break ;
	}
}

int main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("%sWrong number of arguments!\n"
			"Usage:\n./philo number_of_philosophers "
			"time_to_die time_to_eat time_to_sleep "
			"[number_of_times_each_philosopher_must_eat]\n%s", RED, NC);
		return (0);
	}
	if (!init_philo(&data, argv, argc))
	{
		printf("%sError%s\n", RED, NC);
		return (0);
	}
	return (0);
}