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

	gettimeofday(&te, NULL);
	long long milliseconds = te. tv_sec*1000LL + te. tv_usec/1000;
	if (data)
		return (milliseconds - data->sim_start);
	return (milliseconds);
}

char	check_if_someone_died(t_data *data)
{
	int	i;

	//pthread_mutex_lock(&data->check_dead);
	i = 0;
	while (data->philo[i].feed_state[0] != 1 && i < data->number_of_philosophers)
		i++;
	if (i != data->number_of_philosophers)
	{
		//pthread_mutex_unlock(&data->check_dead);
		return (TRUE);
	}
	//pthread_mutex_unlock(&data->check_dead);
	return (FALSE);
}

void	*count_to_death(void *arg)
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
		if (check_if_someone_died(data))
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
			pthread_mutex_lock(&data->check_dead);
			if (check_if_someone_died(data))
			{
				pthread_mutex_unlock(&data->check_dead);
				unlock(&(data->forks[left_fork]),
					   &(data->forks[right_fork]), &(data->philo[my_num].timer));
				return (NULL);
			}
			memset(data->philo[my_num].feed_state, 1, 1);
			printf("%s%lld: %d is dead%s\n", RED, cur_time(data), my_num + 1, NC);
			pthread_mutex_unlock(&data->check_dead);
			unlock(&(data->forks[left_fork]),
				   &(data->forks[right_fork]), &(data->philo[my_num].timer));
			return (NULL);
		}
		pthread_mutex_unlock(&(data->philo[my_num].timer));
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
	left_fork = my_num - 1;
	if (my_num == data->number_of_philosophers)
		right_fork = 0;
	else
		right_fork = my_num;
	pthread_mutex_init(&data->philo[my_num - 1].timer, NULL);
	pthread_create(&timer, NULL, count_to_death, &(t_point){my_num, data});
	while (TRUE)
	{
		if (!_think(data, my_num))
			return (NULL);
		if (!_eat(data, my_num, left_fork, right_fork))
			return (NULL);
		if (!_sleep(data, my_num, left_fork, right_fork))
			return (NULL);
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