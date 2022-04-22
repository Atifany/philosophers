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

void	*wait_till_everyone_eats(void *arg)
{
	t_transfer	*info;
	int			i;

	info = (t_transfer *)arg;
	usleep(50000);
	i = 0;
	while (i++ < info->data->number_of_philosophers)
		sem_wait(info->philos_full);
	sem_wait(info->sem_log);
	ft_printf("%s%u: every philosopher has eaten at least %d times\n%s",
		GRN, cur_time(info->data),
		info->data->times_each_philosopher_must_eat, NC);
	sem_post(info->sem_end);
	return (NULL);
}

static void	parent_and_philos(int *id, int my_num, t_transfer *info)
{
	pthread_t	tid;
	int			i;

	if (my_num == info->data->number_of_philosophers)
	{
		if (info->data->times_each_philosopher_must_eat != -1)
			pthread_create(&tid, NULL, wait_till_everyone_eats, info);
		sem_wait(info->sem_end);
		sem_wait(info->sem_end);
		i = 0;
		while (i < info->data->number_of_philosophers)
			kill(id[i++], SIGKILL);
	}
	else
	{
		sem_wait(info->philos_full);
		info->my_num = my_num;
		philosopher(info);
	}
}

static void	run_philosphers(t_data *data, t_transfer *info)
{
	int			my_num;
	int			*id;

	my_num = 0;
	id = (int *)malloc(4 * data->number_of_philosophers);
	destroy_sems(info);
	sem_opens(data, info);
	while (my_num < data->number_of_philosophers)
	{
		info->t_philo.last_meal = cur_time(data);
		info->t_philo.times_eaten = 0;
		id[my_num] = fork();
		if (!id[my_num])
			break ;
		my_num++;
		usleep(10);
	}
	info->data = data;
	parent_and_philos(id, my_num, info);
	free(id);
}

int	main(int argc, char **argv)
{
	t_transfer	info;
	t_data		data;

	if (argc != 5 && argc != 6)
	{
		ft_printf("%sWrong number of arguments!\n"
			"Usage:\n./philo_bonus number_of_philosophers "
			"time_to_die time_to_eat time_to_sleep "
			"[number_of_times_each_philosopher_must_eat]\n%s", RED, NC);
		return (0);
	}
	if (!init_philo(&data, argv, argc))
	{
		ft_printf("%sError%s\n", RED, NC);
		return (0);
	}
	if (data.times_each_philosopher_must_eat == 0)
	{
		ft_printf("%s%u: every philosopher has eaten at least %d times\n%s",
			GRN, cur_time(&data),
			data.times_each_philosopher_must_eat, NC);
		return (0);
	}
	run_philosphers(&data, &info);
	return (0);
}
