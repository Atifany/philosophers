/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:14:25 by atifany           #+#    #+#             */
/*   Updated: 2022/03/21 15:14:26 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static char	death_timer(t_data *data, int my_num)
{
	long long	last_meal_save;
	long long	timestamp;

	last_meal_save = data->philo[my_num - 1].last_meal;
	usleep(1000);
	while (!data->dead
		&& cur_time(data) - data->philo[my_num - 1].last_meal
		< data->time_to_die / 1000
		&& last_meal_save == data->philo[my_num - 1].last_meal)
		usleep(1000);
	timestamp = cur_time(data);
	pthread_mutex_lock(&(data->philo[my_num - 1].timer));
	if (last_meal_save == data->philo[my_num - 1].last_meal)
	{
		pthread_mutex_lock(&(data->log_queue));
		if (!data->dead)
			printf("%s%llu: %d is dead\n%s", RED, timestamp, my_num, NC);
		data->dead = TRUE;
		pthread_mutex_unlock(&(data->log_queue));
		return (FALSE);
	}
	if (data->dead)
		return (FALSE);
	pthread_mutex_unlock(&(data->philo[my_num - 1].timer));
	return (TRUE);
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
	{
		if (!death_timer(data, my_num))
		{
			unlock(&(data->forks[left_fork]), &(data->forks[right_fork]),
				&(data->philo[my_num - 1].timer));
			return (NULL);
		}
	}
}

static void	philo_life_cycle(void *arg, t_data *data, int my_num)
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
}

void	*philosopher(void *arg)
{
	int			my_num;
	t_point		arg_tmp;
	t_data		*data;
	pthread_t	timer;

	arg_tmp = (*(t_point *)arg);
	data = arg_tmp.data;
	my_num = arg_tmp.num + 1;
	pthread_create(&timer, NULL, timer_to_die, arg);
	data->philo[my_num - 1].last_meal = cur_time(data);
	pthread_mutex_init(&data->philo[my_num - 1].timer, NULL);
	philo_life_cycle(arg, data, my_num);
	pthread_join(timer, NULL);
	return (NULL);
}

int	main(int argc, char **argv)
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
	init_philo(&data, argv, argc);
	free(data.philo);
	free(data.forks);
	return (0);
}
