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

static void	*count_to_death(void *arg)
{
	int			my_num;
	t_point		arg_tmp;
	t_data		*data;

	arg_tmp = (*(t_point *)arg);
	data = arg_tmp.data;
	my_num = arg_tmp.num - 1;
	usleep(data->time_to_die);
	if (data->is_dead[my_num] != 2)
	{
		printf("%s%lld: %d is dead%s\n", RED, cur_time(data), my_num + 1, NC);
		memset(data->is_dead + my_num, 1, 1);
	}
	return (NULL);
}

static void	*philosopher(void *arg)
{
	int			left_fork;
	int			right_fork;
	int			my_num;
	char		first_entrance;
	long long	last_meal;
	t_point		arg_tmp;
	t_data		*data;
	pthread_t	timer;

	first_entrance = TRUE;
	arg_tmp = (*(t_point *)arg);
	data = arg_tmp.data;
	my_num = arg_tmp.num + 1;
	//printf("%s%lld: My number is %d%s\n", MAG, cur_time(data), my_num, NC);
	last_meal = cur_time(data);
	left_fork = my_num - 1;
	if (my_num == data->number_of_philosophers)
		right_fork = 0;
	else
		right_fork = my_num;
	while (TRUE)
	{
		if (!first_entrance)
			printf("%s%lld: %d is thinking%s\n", CYN, cur_time(data), my_num, NC);
		first_entrance = FALSE;
		pthread_mutex_lock(&(data->forks[left_fork]));
		pthread_mutex_lock(&(data->forks[right_fork]));
		if (data->is_dead[my_num] != 1)
			memset(data->is_dead + my_num, 2, 1);
		printf("%s%lld: %d is eating%s\n", CYN, cur_time(data), my_num, NC);
		usleep(data->time_to_eat);
		pthread_mutex_unlock(&(data->forks[left_fork]));
		pthread_mutex_unlock(&(data->forks[right_fork]));
		pthread_create(&timer, NULL, count_to_death, &(t_point){my_num, data});
		printf("%s%lld: %d is sleeping%s\n", CYN, cur_time(data), my_num, NC);
		usleep(data->time_to_sleep);
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

	data.is_dead = malloc(data.number_of_philosophers);
	memset(data.is_dead, 0, data.number_of_philosophers);
	data.sim_start = cur_time(NULL);
	data.number_of_philosophers = ft_atoi(argv[1]);
	data.time_to_die = ft_atoi(argv[2]) * 1000;
	data.time_to_eat = ft_atoi(argv[3]) * 1000;
	data.time_to_sleep = ft_atoi(argv[4]) * 1000;
	data.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data.number_of_philosophers);
	if (argc == 6)
		data.number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	//printf("%sInit fork number %d with %d%s\n", CYN, 0, data.forks[0], NC);
	//printf("%sInit fork number %d with %d%s\n", CYN, 1, data.forks[1], NC);
	//printf("%sInit fork number %d with %d%s\n", CYN, 2, data.forks[2], NC);
	//printf("%sInit fork number %d with %d%s\n", CYN, 3, data.forks[3], NC);

	sleep(1);
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
		usleep(50);
		i++;
	}
	printf("started every philosopher!\n");
	i = 0;
	while (data.is_dead[i] != 1)
	{
		i++;
		if (i == data.number_of_philosophers)
			i = 0;
		usleep(10);
	}
	free(thread_ids);
	free(data.is_dead);
	free(data.forks);
	return (0);
}