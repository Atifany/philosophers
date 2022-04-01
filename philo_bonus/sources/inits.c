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

static void	destroy_sems(sem_t *sem_1, sem_t *sem_2, sem_t *sem_3)
{
	sem_unlink("forks");
	sem_close(sem_1);
	sem_unlink("logs");
	sem_close(sem_2);
	sem_unlink("gettime");
	sem_close(sem_3);
}

static void	run_philosphers(t_data *data)
{
	int		my_num;
	int		id;
	sem_t	*sem_forks;
	sem_t	*sem_logs;
	sem_t	*sem_gettime;

	sem_forks = sem_open("forks", O_CREAT | O_EXCL, 0644, data->number_of_philosophers);
	sem_logs = sem_open("logs", O_CREAT | O_EXCL, 0644, 1);
	sem_gettime = sem_open("gettime", O_CREAT | O_EXCL, 0644, 1);
	if (errno == EEXIST)
	{
		destroy_sems(sem_forks, sem_logs, sem_gettime);
		sem_forks = sem_open("forks", O_CREAT | O_EXCL, 0644, data->number_of_philosophers);
		sem_logs = sem_open("logs", O_CREAT | O_EXCL, 0644, 1);
		sem_gettime = sem_open("gettime", O_CREAT | O_EXCL, 0644, 1);
	}
	my_num = 0;
	while (my_num < data->number_of_philosophers)
	{
		data->philo[my_num].last_meal = cur_time(data, sem_gettime);
		data->philo[my_num].times_eaten = 0;
		id = fork();
		if (id)
			data->philo[my_num].my_pid = id;
		else
			break ;
		my_num++;
		usleep(10);
	}
	if (id)
	{
		while (!waitpid(data->philo[0].my_pid, NULL, WNOHANG | WUNTRACED));
		sem_wait(sem_logs);
		printf("%sI've been waiting them the whole day! %s\n", YEL, NC);
		sem_post(sem_logs);
		destroy_sems(sem_forks, sem_logs, sem_gettime);
	}
	else
	{
		philosopher(data, my_num, sem_forks, sem_logs, sem_gettime);
		printf("%s%d exited!%s\n", RED, my_num + 1, NC);
		exit(0);
	}
}

char	init_philo(t_data *data, char **args, int argc)
{
	int	i;

	i = 1;
	while (i < argc)
		if (!is_valid_int(args[i++]))
			return (FALSE);
	data->dead = 0;
	data->number_of_philosophers = ft_atoi(args[1]);
	data->philososphers_hungry = data->number_of_philosophers;
	data->time_to_die = ft_atoi(args[2]);
	data->time_to_eat = ft_atoi(args[3]);
	data->time_to_sleep = ft_atoi(args[4]);
	if (argc == 6)
		data->times_each_philosopher_must_eat = ft_atoi(args[5]);
	data->philo = (t_philo *)
		malloc(sizeof(t_philo) * data->number_of_philosophers);
	//init_locks(data);
	data->sim_start = cur_time(NULL, NULL);
	run_philosphers(data);
	return (TRUE);
}
