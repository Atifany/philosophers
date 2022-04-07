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

static char	is_valid_int(char *n)
{
	int	i;

	i = 0;
	while (i < 10 && n[i] != 0)
	{
		if (n[i] > '9' || '0' > n[i])
			return (FALSE);
		i++;
	}
	if (n[i] != 0)
		return (FALSE);
	return (TRUE);
}

static void	destroy_sems(sem_t *sem_1, sem_t *sem_2)
{
	sem_unlink("forks");
	sem_close(sem_1);
	sem_unlink("logs");
	sem_close(sem_2);
}

static void	run_philosphers(t_data *data, t_transfer *info)
{
	int		my_num;
	int		*id;
	int		i;

	id = (int *)malloc(sizeof(int) * data->number_of_philosophers);
	info->sem_forks = sem_open("forks", O_CREAT | O_EXCL, 0644, data->number_of_philosophers);
	info->sem_logs = sem_open("logs", O_CREAT | O_EXCL, 0644, 1);
	if (errno == EEXIST)
	{
		destroy_sems(info->sem_forks, info->sem_logs);
		info->sem_forks = sem_open("forks", O_CREAT | O_EXCL, 0644, data->number_of_philosophers);
		info->sem_logs = sem_open("logs", O_CREAT | O_EXCL, 0644, 1);
	}
	my_num = 0;
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
	if (my_num == data->number_of_philosophers)
	{
		while (!waitpid(-1, NULL, WNOHANG | WUNTRACED));
		i = 0;
		while (i < data->number_of_philosophers)
			kill(id[i++], SIGKILL);
		destroy_sems(info->sem_forks, info->sem_logs);
	}
	else
	{
		info->my_num = my_num;
		philosopher(info);
		ft_printf("%s%d exited!%s\n", RED, my_num + 1, NC);
	}
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
	if (argc == 6)
		data->times_each_philosopher_must_eat = ft_atoi(args[5]);
	data->sim_start = cur_time(NULL);
	run_philosphers(data, &info);
	return (TRUE);
}
