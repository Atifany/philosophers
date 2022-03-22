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

static long long	cur_time(t_data *data)
{
	struct timeval	te;

	gettimeofday(&te, NULL);
	long long milliseconds = te. tv_sec*1000LL + te. tv_usec/1000;
	if (data)
		return (milliseconds - data->sim_start);
	return (milliseconds);
}

static char	check_if_someone_died(t_data data)
{
	int	i;

	i = 0;
	while (data.philo[i].feed_state[0] != 1 && i < data.number_of_philosophers)
		i++;
	if (i != data.number_of_philosophers)
		return (TRUE);
	return (FALSE);
}

static void	*count_to_death(void *arg)
{
	int		i;
	int		my_num;
	t_point	arg_tmp;
	t_data	*data;

	arg_tmp = (*(t_point *)arg);
	data = arg_tmp.data;
	my_num = arg_tmp.num - 1;
	//printf("%s%lld: %d has started starving%s\n", YEL, cur_time(data), my_num + 1, NC);
	usleep(data->time_to_die);
	pthread_mutex_lock(&data->log_queue);
	//printf("%sLog of %d: %d|%llu|%llu|%llu|%d|%d%s\n", YEL, my_num + 1,
	//	   cur_time(data) - data->philo[my_num].last_meal >= data->time_to_die / 1000,
	//	   cur_time(data), data->philo[my_num].last_meal, data->time_to_die / 1000,
	//	   check_if_someone_died(*data), data->philo[my_num].feed_state[0], NC);
	if (cur_time(data) - data->philo[my_num].last_meal >= data->time_to_die / 1000 &&
		!check_if_someone_died(*data) && data->philo[my_num].feed_state[0] == 0)
	{
		i = 0;
		while (data->philo[i].feed_state[0] != 1 && i < data->number_of_philosophers &&
			printf("%s%d data.philo.feed_state: %d%s\n", YEL, i+1, data->philo[i].feed_state[0], NC))
			i++;
		printf("%s%lld: %d is dead%s\n", RED, cur_time(data), my_num + 1, NC);
		memset(data->philo[my_num].feed_state, 1, 1);
	}
	pthread_mutex_unlock(&data->log_queue);
	return (NULL);
}

static char	_think(t_data *data, int my_num)
{
	pthread_mutex_lock(&data->log_queue);
	if (check_if_someone_died(*data))
		return (FALSE);
	printf("%s%lld: %d is thinking%s\n", CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&data->log_queue);
	return (TRUE);
}

static char	_eat(t_data *data, int my_num)
{
	int			left_fork;
	int			right_fork;
	pthread_t	timer;

	left_fork = my_num - 1;
	if (my_num == data->number_of_philosophers)
		right_fork = 0;
	else
		right_fork = my_num;
	pthread_mutex_lock(&(data->forks[left_fork]));
	pthread_mutex_lock(&(data->forks[right_fork]));
	pthread_mutex_lock(&data->log_queue);
	if (check_if_someone_died(*data))
		return (FALSE);
	if (data->philo[my_num - 1].feed_state[0] != 1)
		memset(data->philo[my_num - 1].feed_state, 2, 1);
	printf("%s%lld: %d is eating%s\n", CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&data->log_queue);
	usleep(data->time_to_eat);
	pthread_mutex_lock(&data->log_queue);
	data->philo[my_num - 1].last_meal = cur_time(data);
	if (data->philo[my_num - 1].feed_state[0] != 1)
		memset(data->philo[my_num - 1].feed_state, 0, 1);
	pthread_create(&timer, NULL, count_to_death, &(t_point){my_num, data});
	pthread_mutex_unlock(&data->log_queue);
	pthread_mutex_unlock(&(data->forks[left_fork]));
	pthread_mutex_unlock(&(data->forks[right_fork]));
	return (TRUE);
}

static char	_sleep(t_data *data, int my_num)
{
	pthread_mutex_lock(&data->log_queue);
	if (check_if_someone_died(*data))
		return (FALSE);
	printf("%s%lld: %d is sleeping%s\n", CYN, cur_time(data), my_num, NC);
	pthread_mutex_unlock(&data->log_queue);
	usleep(data->time_to_sleep);
	return (TRUE);
}

static void	*philosopher(void *arg)
{
	int			my_num;
	t_point		arg_tmp;
	t_data		*data;
	pthread_t	timer;

	arg_tmp = (*(t_point *)arg);
	data = arg_tmp.data;
	my_num = arg_tmp.num + 1;
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
	int			i;
	pthread_t	*thread_ids;
	t_data		data;

	if (argc != 5 && argc != 6)
	{
		printf("%sWrong number of arguments!\nUsage:\n./philo number_of_philosophers "
			   "time_to_die time_to_eat time_to_sleep "
			   "[number_of_times_each_philosopher_must_eat]\n%s", RED, NC);
		return (0);
	}

	pthread_mutex_init(&data.log_queue, NULL);
	data.number_of_philosophers = ft_atoi(argv[1]);
	data.time_to_die = ft_atoi(argv[2]) * 1000;
	data.time_to_eat = ft_atoi(argv[3]) * 1000;
	data.time_to_sleep = ft_atoi(argv[4]) * 1000;
	data.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data.number_of_philosophers);
	if (argc == 6)
		data.number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);

	data.philo = (t_philo *)malloc(sizeof(t_philo) * data.number_of_philosophers);
	i = 0;
	while (i < data.number_of_philosophers)
	{
		data.philo[i].feed_state[0] = 0;
		data.philo[i].last_meal = 0;
		i++;
	}
	sleep(1);
	data.sim_start = cur_time(NULL);
	thread_ids = (pthread_t *)malloc(sizeof(pthread_t) * data.number_of_philosophers);
	i = 0;
	while (i < data.number_of_philosophers)
	{
		pthread_mutex_init(&(data.forks[i]), NULL);
		i++;
	}
	i = 0;
	while (i < data.number_of_philosophers)
	{
		pthread_create(&(thread_ids[i]), NULL, philosopher, &(t_point){i, &data});
		usleep(5);
		i++;
	}
	printf("%sstarted every philosopher!\n%s", YEL, NC);
	while (!check_if_someone_died(data));
	sleep(1);
	free(thread_ids);
	free(data.philo);
	free(data.forks);
	return (0);
}