/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 17:44:26 by atifany           #+#    #+#             */
/*   Updated: 2022/04/12 17:44:27 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	destroy_sems(t_transfer *info)
{
	if (info->sem_forks == SEM_FAILED)
	{
		sem_unlink("forks");
		sem_close(info->sem_forks);
	}
	if (info->sem_logs == SEM_FAILED)
	{
		sem_unlink("logs");
		sem_close(info->sem_logs);
	}
	if (info->eating_state == SEM_FAILED)
	{
		sem_unlink("eat_state");
		sem_close(info->eating_state);
	}
	if (info->end == SEM_FAILED)
	{
		sem_unlink("end");
		sem_close(info->end);
	}
	if (info->philos_full == SEM_FAILED)
	{
		sem_unlink("philos_full");
		sem_close(info->philos_full);
	}
}

void	sem_opens(t_data *data, t_transfer *info)
{
	info->sem_forks
		= sem_open("forks", O_CREAT | O_EXCL, 7777,
			data->number_of_philosophers);
	info->sem_logs
		= sem_open("logs", O_CREAT | O_EXCL, 7777, 1);
	info->eating_state
		= sem_open("eat_state", O_CREAT | O_EXCL, 7777, 1);
	info->end
		= sem_open("end", O_CREAT | O_EXCL, 7777, 1);
	info->philos_full
		= sem_open("philos_full", O_CREAT | O_EXCL, 7777,
			data->number_of_philosophers);
}
