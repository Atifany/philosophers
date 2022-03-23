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

char	check_if_someone_died(t_data data)
{
	int	i;

	i = 0;
	while (data.philo[i].feed_state[0] != 1 && i < data.number_of_philosophers)
		i++;
	if (i != data.number_of_philosophers)
		return (TRUE);
	return (FALSE);
}

void	*count_to_death(void *arg)
{
	//int		i;
	int		my_num;
	t_point	arg_tmp;
	t_data	*data;

	arg_tmp = (*(t_point *)arg);
	data = arg_tmp.data;
	my_num = arg_tmp.num - 1;
	//printf("%s%lld: %d has started starving%s\n", YEL, cur_time(data), my_num + 1, NC);
	while (TRUE)
	{
		//usleep(data->time_to_die);
		usleep(1000);
		while (cur_time(data) - data->philo[my_num].last_meal < data->time_to_die / 1000
			&& cur_time(data) <= data->philo[my_num].last_meal)
			usleep(1000);
		//printf("%llu|%llu|%llu\n", cur_time(data), data->philo[my_num].last_meal, data->time_to_die / 1000);
		//printf("%s%llu: %d is waiting unlock to check death!%s\n", MAG, cur_time(data), my_num + 1, NC);
		pthread_mutex_lock(&data->philo[my_num].timer);
		if (check_if_someone_died(*data))
		{
			pthread_mutex_unlock(&data->philo[my_num].timer);
			return (NULL);
		}
		if (cur_time(data) - data->philo[my_num].last_meal >= data->time_to_die / 1000 &&
			data->philo[my_num].feed_state[0] == 0)
		{
			//printf("%d is definitely going to die!\n", my_num + 1);
			if (check_if_someone_died(*data))
			{
				pthread_mutex_unlock(&data->philo[my_num].timer);
				return (NULL);
			}
			//pthread_mutex_lock(&data->log_queue);
			memset(data->philo[my_num].feed_state, 1, 1);
			printf("%s%lld: %d is dead%s\n", RED, cur_time(data), my_num + 1, NC);
			//pthread_mutex_unlock(&data->log_queue);
			pthread_mutex_unlock(&data->philo[my_num].timer);
			return (NULL);
		}
		//printf("%d finished this timer safely!\n", my_num + 1);
		pthread_mutex_unlock(&data->philo[my_num].timer);
	}
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
	pthread_mutex_init(&data->philo[my_num - 1].timer, NULL);
	pthread_create(&timer, NULL, count_to_death, &(t_point){my_num, data});
	while (TRUE)
	{
		if (!_think(data, my_num))
			return (NULL);
		if (!_eat(data, my_num))
			return (NULL);
		if (!_sleep(data, my_num))
			return (NULL);
	}
}

int	main(int argc, char **argv)
{
	t_data		data;

	if (argc != 5 && argc != 6)
	{
		printf("%sWrong number of arguments!\nUsage:\n./philo number_of_philosophers "
			   "time_to_die time_to_eat time_to_sleep "
			   "[number_of_times_each_philosopher_must_eat]\n%s", RED, NC);
		return (0);
	}
	init_philo(&data, argv, argc);
	printf("%sstarted every philosopher!\n%s", YEL, NC);
	//while (!check_if_someone_died(data))
	//	usleep(5000);
	int i = 0;
	while (i < data.number_of_philosophers &&
		   printf("%s%d data.philo.feed_state: %d%s\n", YEL, i+1, data.philo[i].feed_state[0], NC))
		i++;
	usleep(data.time_to_die + 1000);
	free(data.philo);
	free(data.forks);
	return (0);
}