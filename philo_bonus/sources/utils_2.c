/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 15:37:39 by atifany           #+#    #+#             */
/*   Updated: 2022/04/16 15:37:40 by atifany          ###   ########.fr       */
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
