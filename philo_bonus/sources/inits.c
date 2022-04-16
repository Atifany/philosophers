/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:42:02 by atifany           #+#    #+#             */
/*   Updated: 2022/04/01 14:42:03 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

static void	init_sems(t_data *data, t_transfer *info)
{
	sem_opens(data, info);
	if (errno == EEXIST)
	{
		destroy_sems(info);
		sem_opens(data, info);
	}
}

void	*wait_till_everyone_eats(void *arg)
{
	t_transfer	*info;
	int			i;

	info = (t_transfer *)arg;
	usleep(50000);
	i = 0;
	while (i++ < info->data->number_of_philosophers)
		sem_wait(info->philos_full);
	usleep(100);
	if (!info->is_end)
	{
		sem_wait(info->sem_logs);
		ft_printf("%s%u: every philosopher has eaten at least %d times\n%s",
			GRN, cur_time(info->data),
			info->data->times_each_philosopher_must_eat, NC);
	}
	info->is_end = TRUE;
	return (NULL);
}

static void	philo_main(int *id, int my_num, t_transfer *info)
{
	pthread_t	tid;
	int			i;

	if (my_num == info->data->number_of_philosophers)
	{
		info->is_end = FALSE;
		if (info->data->times_each_philosopher_must_eat != -1)
			pthread_create(&tid, NULL, wait_till_everyone_eats, info);
		while (!waitpid(-1, NULL, WNOHANG | WUNTRACED) && !info->is_end)
			;
		info->is_end = TRUE;
		i = 0;
		while (i < info->data->number_of_philosophers)
			kill(id[i++], SIGKILL);
		destroy_sems(info);
	}
	else
	{
		info->my_num = my_num;
		philosopher(info);
		/*ft_printf("%s%d exited!%s\n", RED, my_num + 1, NC);
		i = 0;
		while (i++ < info->data->number_of_philosophers)
			sem_post(info->philos_full);*/
	}
}

static void	run_philosphers(t_data *data, t_transfer *info)
{
	int			my_num;
	int			*id;

	my_num = 0;
	id = (int *)malloc(4 * data->number_of_philosophers);
	init_sems(data, info);
	while (my_num < data->number_of_philosophers)
	{
		info->t_philo.last_meal = cur_time(data);
		info->t_philo.times_eaten = 0;
		id[my_num] = fork();
		if (!id[my_num])
		{
			sem_wait(info->philos_full);
			break ;
		}
		my_num++;
		usleep(10);
	}
	info->data = data;
	philo_main(id, my_num, info);
	free(id);
}

char	init_philo(t_data *data, char **args, int argc)
{
	t_transfer	info;
	int			i;

	i = 1;
	while (i < argc)
		if (!is_valid_int(args[i++]))
			return (FALSE);
	data->number_of_philosophers = ft_atoi(args[1]);
	data->philososphers_hungry = data->number_of_philosophers;
	data->time_to_die = ft_atoi(args[2]);
	data->time_to_eat = ft_atoi(args[3]);
	data->time_to_sleep = ft_atoi(args[4]);
	data->times_each_philosopher_must_eat = -1;
	if (argc == 6)
		data->times_each_philosopher_must_eat = ft_atoi(args[5]);
	data->sim_start = cur_time(NULL);
	run_philosphers(data, &info);
	return (TRUE);
}
