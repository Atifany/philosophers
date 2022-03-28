/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 19:27:18 by atifany           #+#    #+#             */
/*   Updated: 2022/03/28 19:27:19 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	wait_to_starve(t_data *data, int my_num, int last_meal_save)
{
	usleep(1000);
	while (!data->dead
		&& cur_time(data) - data->philo[my_num - 1].last_meal
		< data->time_to_die / 1000
		&& last_meal_save == data->philo[my_num - 1].last_meal)
		usleep(1000);
}

static char	death_timer(t_data *data, int my_num, int left_fork, int right_fork)
{
	long long	last_meal_save;
	long long	timestamp;

	last_meal_save = data->philo[my_num - 1].last_meal;
	wait_to_starve(data, my_num, last_meal_save);
	timestamp = cur_time(data);
	pthread_mutex_lock(&(data->philo[my_num - 1].timer));
	if (cur_time(data) - data->philo[my_num - 1].last_meal
		>= data->time_to_die / 1000
		&& last_meal_save == data->philo[my_num - 1].last_meal)
	{
		pthread_mutex_lock(&(data->log_queue));
		if (!data->dead)
			printf("%s%llu: %d is dead\n%s", RED, timestamp, my_num, NC);
		data->dead = TRUE;
		pthread_mutex_unlock(&(data->log_queue));
		unlock(&(data->forks[left_fork]),
			&(data->forks[right_fork]), &(data->philo[my_num - 1].timer));
		return (FALSE);
	}
	pthread_mutex_unlock(&(data->philo[my_num - 1].timer));
	if (data->dead)
		return (FALSE);
	return (TRUE);
}

void	*philo_life_cycle(void *arg, t_data *data, int my_num)
{
	int		left_fork;
	int		right_fork;
	char	flag;

	left_fork = my_num - 1;
	if (my_num == data->number_of_philosophers)
		right_fork = 0;
	else
		right_fork = my_num;
	flag = TRUE;
	while (flag)
	{
		if (!_think(data, my_num))
			flag = FALSE;
		if (flag && !_eat(data, my_num, left_fork, right_fork))
			flag = FALSE;
		if (flag && !_sleep(data, my_num, left_fork, right_fork))
			flag = FALSE;
	}
	free(arg);
	return (NULL);
}

void	*timer_to_die(void *arg)
{
	int			left_fork;
	int			right_fork;
	t_point		arg_tmp;
	t_data		*data;
	int			my_num;

	arg_tmp = (*(t_point *)arg);
	data = arg_tmp.data;
	my_num = arg_tmp.num + 1;
	left_fork = my_num - 1;
	if (my_num == data->number_of_philosophers)
		right_fork = 0;
	else
		right_fork = my_num;
	while (TRUE)
		if (!death_timer(data, my_num, left_fork, right_fork))
			return (NULL);
}
