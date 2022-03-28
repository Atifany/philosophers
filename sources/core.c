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

# include "../philo.h"

long long	cur_time(t_data *data)
{
	struct timeval	te;

	if (data)
		pthread_mutex_lock(&data->get_time);
	gettimeofday(&te, NULL);
	long long milliseconds = te. tv_sec*1000LL + te. tv_usec/1000;
	if (data)
		pthread_mutex_unlock(&data->get_time);
	if (data)
		return (milliseconds - data->sim_start);
	return (milliseconds);
}

/*void	*count_to_death(void *arg)
{
	int		left_fork;
	int		right_fork;
	int		my_num;
	t_point	arg_tmp;
	t_data	*data;

	arg_tmp = (*(t_point *)arg);
	data = arg_tmp.data;
	my_num = arg_tmp.num - 1;
	left_fork = my_num;
	if (my_num + 1 == data->number_of_philosophers)
		right_fork = 0;
	else
		right_fork = my_num;
	while (TRUE)
	{
		usleep(1000);
		while (cur_time(data) - data->philo[my_num].last_meal < data->time_to_die / 1000
			&& cur_time(data) <= data->philo[my_num].last_meal)
			usleep(1000);
		pthread_mutex_lock(&(data->philo[my_num].timer));
		pthread_mutex_lock(&data->check_dead);
		if (data->dead)
		{
			pthread_mutex_unlock(&data->check_dead);
			unlock(&(data->forks[left_fork]),
				   &(data->forks[right_fork]), &data->philo[my_num].timer);
			return (NULL);
		}
		pthread_mutex_unlock(&data->check_dead);
		if (cur_time(data) - data->philo[my_num].last_meal >= data->time_to_die / 1000 &&
			data->philo[my_num].feed_state[0] == 0)
		{
			//printf("%d lols\n", my_num + 1);
			pthread_mutex_lock(&data->check_dead);
			//printf("%d keks\n", my_num + 1);
			if (data->dead)
			{
				//printf("%d good ending\n", my_num + 1);
				pthread_mutex_unlock(&data->check_dead);
				unlock(&(data->forks[left_fork]),
					   &(data->forks[right_fork]), &(data->philo[my_num].timer));
				return (NULL);
			}
			data->dead = 1;
			printf("%s%lld: %d is dead%s\n", RED, cur_time(data), my_num + 1, NC);
			pthread_mutex_unlock(&data->check_dead);
			unlock(&(data->forks[left_fork]),
				   &(data->forks[right_fork]), &(data->philo[my_num].timer));
			return (NULL);
		}
		pthread_mutex_unlock(&(data->philo[my_num].timer));
	}
}*/

void	*timer_to_die(void *arg)
{
	long long	last_meal_save;
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
		// this timer kinda scary
		last_meal_save = data->philo[my_num - 1].last_meal;
		usleep(1000);
		while (!data->dead
			&& cur_time(data) - data->philo[my_num - 1].last_meal < data->time_to_die / 1000
			&& last_meal_save == data->philo[my_num - 1].last_meal)
			usleep(1000);
		// if eating than wait till philo finishes eating via endless loop. ahh... howdiho! pamagi!
		if (cur_time(data) - data->philo[my_num - 1].last_meal >= data->time_to_die / 1000
			&& last_meal_save == data->philo[my_num - 1].last_meal)
		{
			//pthread_mutex_lock(&(data->philo[my_num - 1].timer));
			pthread_mutex_lock(&(data->log_queue));
			if (!data->dead)
				printf("%s%llu: %d is dead\nlast meal in: %llu\ntime to die is: %llu%s\n",
					   RED, cur_time(data), my_num, data->philo[my_num - 1].last_meal,
					   data->time_to_die / 1000, NC);
			data->dead = TRUE;
			pthread_mutex_unlock(&(data->log_queue));
			unlock(&(data->forks[left_fork]),
				   &(data->forks[right_fork]), &(data->philo[my_num - 1].timer));
			return (NULL);
		}
		pthread_mutex_unlock(&(data->philo[my_num - 1].timer));
		if (data->dead)
			return (NULL);
	}
}

void	*philosopher(void *arg)
{
	int			left_fork;
	int			right_fork;
	int			my_num;
	t_point		arg_tmp;
	t_data		*data;
	pthread_t	timer;

	arg_tmp = (*(t_point *)arg);
	data = arg_tmp.data;
	my_num = arg_tmp.num + 1;
	pthread_create(&timer, NULL, timer_to_die, arg);
	left_fork = my_num - 1;
	if (my_num == data->number_of_philosophers)
		right_fork = 0;
	else
		right_fork = my_num;
	data->philo[my_num - 1].last_meal = cur_time(data);
	pthread_mutex_init(&data->philo[my_num - 1].timer, NULL);
	while (TRUE)
	{
		if (!_think(data, my_num))
		{
			free(arg);
			return (NULL);
		}
		if (!_eat(data, my_num, left_fork, right_fork))
		{
			free(arg);
			return (NULL);
		}
		if (!_sleep(data, my_num, left_fork, right_fork))
		{
			free(arg);
			return (NULL);
		}
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("%sWrong number of arguments!\nUsage:\n./philo number_of_philosophers "
			   "time_to_die time_to_eat time_to_sleep "
			   "[number_of_times_each_philosopher_must_eat]\n%s", RED, NC);
		return (0);
	}
	init_philo(&data, argv, argc);
	free(data.philo);
	free(data.forks);
	return (0);
}