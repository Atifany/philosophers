/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:15:14 by atifany           #+#    #+#             */
/*   Updated: 2022/03/23 15:15:15 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	init_locks(t_data *data)
{
	int	i;

	pthread_mutex_init(&data->get_time, NULL);
	pthread_mutex_init(&data->log_queue, NULL);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_init(&(data->forks[i]), NULL);
		i++;
	}
}

static void	run_philosphers(t_data *data)
{
	t_point		*message;
	pthread_t	*thread_ids;
	int			i;

	thread_ids = (pthread_t *)
		malloc(sizeof(pthread_t) * data->number_of_philosophers);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		message = (t_point *)malloc(sizeof(t_point));
		message->data = data;
		message->num = i;
		pthread_create(&(thread_ids[i]), NULL, philosopher, message);
		i++;
	}
	i = 0;
	while (i < data->number_of_philosophers)
		pthread_join(thread_ids[i++], NULL);
	if (data->philososphers_hungry == 0)
		printf("%sEach philospher has eaten at least %d times%s\n",
			GRN, data->times_each_philosopher_must_eat, NC);
	free(thread_ids);
}

void	init_philo(t_data *data, char **args, int argc)
{
	int			i;

	//add validation!!!
	data->dead = 0;
	data->number_of_philosophers = ft_atoi(args[1]);
	data->philososphers_hungry = data->number_of_philosophers;
	data->time_to_die = ft_atoi(args[2]) * 1000;
	data->time_to_eat = ft_atoi(args[3]) * 1000;
	data->time_to_sleep = ft_atoi(args[4]) * 1000;
	data->forks = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t) * data->number_of_philosophers);
	if (argc == 6)
		data->times_each_philosopher_must_eat = ft_atoi(args[5]);
	data->philo = (t_philo *)
		malloc(sizeof(t_philo) * data->number_of_philosophers);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		data->philo[i].times_eaten = 0;
		data->philo[i].last_meal = 0;
		i++;
	}
	init_locks(data);
	data->sim_start = cur_time(NULL);
	run_philosphers(data);
}
