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

void	destroy_sems(t_transfer *info)
{
	sem_unlink("forks");
	sem_close(info->sem_forks);
	sem_unlink("philos_full");
	sem_close(info->philos_full);
	sem_unlink("/log");
	sem_close(info->sem_log);
	sem_unlink("/eat");
	sem_close(info->sem_eat);
	sem_unlink("/end");
	sem_close(info->sem_end);
}

void	sem_opens(t_data *data, t_transfer *info)
{
	info->sem_forks
		= sem_open("forks", O_CREAT | O_EXCL, 7777,
			data->number_of_philosophers);
	info->philos_full
		= sem_open("philos_full", O_CREAT | O_EXCL, 7777,
			data->number_of_philosophers);
	info->sem_log = sem_open("/log", O_CREAT | O_EXCL, 7777, 1);
	info->sem_eat = sem_open("/eat", O_CREAT | O_EXCL, 7777, 1);
	info->sem_end = sem_open("/end", O_CREAT | O_EXCL, 7777, 1);
}

char	init_philo(t_data *data, char **args, int argc)
{
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
	return (TRUE);
}
