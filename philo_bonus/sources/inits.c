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

static void	destroy_sems(sem_t *sem_1, sem_t *sem_2, sem_t *sem_3, sem_t *sem_4, sem_t *sem_5)
{
	sem_unlink("forks");
	sem_close(sem_1);
	sem_unlink("logs");
	sem_close(sem_2);
	sem_unlink("eat_state");
	sem_close(sem_3);
	sem_unlink("dead_state");
	sem_close(sem_4);
	sem_unlink("philos_full");
	sem_close(sem_5);
}

void	*wait_till_everyone_eats(void *arg)
{
	t_transfer	*info;
	int			i;

	info = (t_transfer *)arg;
	usleep(50000);
	i = 0;
	while (i < info->data->number_of_philosophers)
	{
		sem_wait(info->philos_full);
		i++;
	}
	usleep(100);
	if (!info->is_end)
	{
		sem_wait(info->sem_logs);
		ft_printf("%s%u: every philosopher has eaten at least %d times\n%s", GRN, cur_time(info->data), info->data->times_each_philosopher_must_eat, NC);
	}
	info->is_end = TRUE;
	return (NULL);
}

static void	run_philosphers(t_data *data, t_transfer *info)
{
	pthread_t	tid;
	int			my_num;
	int			*id;
	int			i;

	id = (int *)malloc(sizeof(int) * data->number_of_philosophers);
	info->sem_forks = sem_open("forks", O_CREAT | O_EXCL, 7777, data->number_of_philosophers);
	info->sem_logs = sem_open("logs", O_CREAT | O_EXCL, 7777, 1);
	info->eating_state = sem_open("eat_state", O_CREAT | O_EXCL, 7777, 1);
	info->dead_state = sem_open("dead_state", O_CREAT | O_EXCL, 7777, 1);
	info->philos_full = sem_open("philos_full", O_CREAT | O_EXCL, 7777, data->number_of_philosophers);
	if (errno == EEXIST)
	{
		destroy_sems(info->sem_forks, info->sem_logs, info->eating_state, info->dead_state, info->philos_full);
		info->sem_forks = sem_open("forks", O_CREAT | O_EXCL, 7777, data->number_of_philosophers);
		info->sem_logs = sem_open("logs", O_CREAT | O_EXCL, 7777, 1);
		info->eating_state = sem_open("eat_state", O_CREAT | O_EXCL, 7777, 1);
		info->dead_state = sem_open("dead_state", O_CREAT | O_EXCL, 7777, 1);
		info->philos_full = sem_open("philos_full", O_CREAT | O_EXCL, 7777, data->number_of_philosophers);
	}
	my_num = 0;
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
	if (my_num == data->number_of_philosophers)
	{
		info->is_end = FALSE;
		if (data->times_each_philosopher_must_eat != -1)
			pthread_create(&tid, NULL, wait_till_everyone_eats, info);
		while (!waitpid(-1, NULL, WNOHANG | WUNTRACED) && !info->is_end);
		info->is_end = TRUE;
		i = 0;
		while (i < data->number_of_philosophers)
			kill(id[i++], SIGKILL);
		destroy_sems(info->sem_forks, info->sem_logs, info->eating_state, info->dead_state, info->philos_full);
	}
	else
	{
		info->my_num = my_num;
		philosopher(info);
		ft_printf("%s%d exited!%s\n", RED, my_num + 1, NC);
		i = 0;
		while (i++ < info->data->number_of_philosophers)
			sem_post(info->philos_full);
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
	data->times_each_philosopher_must_eat = -1;
	if (argc == 6)
		data->times_each_philosopher_must_eat = ft_atoi(args[5]);
	data->sim_start = cur_time(NULL);
	run_philosphers(data, &info);
	return (TRUE);
}
