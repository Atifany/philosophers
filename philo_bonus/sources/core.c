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

static void	am_i_dead(t_transfer *info, long long timestamp)
{
	sem_wait(info->dead_state);
	sem_wait(info->sem_logs);
	ft_printf("%s%u: %d is dead\n%s",
		RED, timestamp, info->my_num + 1, NC);
	info->is_dead = TRUE;
	sem_post(info->dead_state);
	sem_post(info->sem_forks);
	sem_post(info->sem_forks);
	sem_post(info->eating_state);
}

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
		sem_wait(info->eating_state);
		if (last_meal_save == info->t_philo.last_meal)
		{
			am_i_dead(info, timestamp);
			return ;
		}
		sem_post(info->eating_state);
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
		if (!_eat(info))
			break ;
		if (++times_eaten == info->data->times_each_philosopher_must_eat)
		{
			sem_wait(info->sem_logs);
			ft_printf("%s%d: %d is full\n%s",
				YEL, cur_time(info->data), info->my_num + 1, NC);
			sem_post(info->philos_full);
			sem_post(info->sem_logs);
		}
		if (!_sleep(info))
			break ;
	}
	return (NULL);
}

void	philosopher(t_transfer *info)
{
	pthread_t	tid;

	info->is_dead = FALSE;
	pthread_create(&tid, NULL, philo_life_cycle, info);
	_clock(info);
	pthread_join(tid, NULL);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		ft_printf("%sWrong number of arguments!\n"
			"Usage:\n./philo number_of_philosophers "
			"time_to_die time_to_eat time_to_sleep "
			"[number_of_times_each_philosopher_must_eat]\n%s", RED, NC);
		return (0);
	}
	if (!init_philo(&data, argv, argc))
	{
		ft_printf("%sError%s\n", RED, NC);
		return (0);
	}
	return (0);
}
