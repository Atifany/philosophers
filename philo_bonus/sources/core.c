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

void	philosopher(t_data *data, int my_num, sem_t *sem_forks, sem_t *sem_logs, sem_t *sem_gettime)
{
	while (TRUE)
	{
		_think(data, my_num, sem_logs, sem_gettime);
		_eat(data, my_num, sem_forks, sem_logs, sem_gettime);
		_sleep(data, my_num, sem_logs, sem_gettime);
	}
}

int main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("%sWrong number of arguments!\n"
			"Usage:\n./philo number_of_philosophers "
			"time_to_die time_to_eat time_to_sleep "
			"[number_of_times_each_philosopher_must_eat]\n%s", RED, NC);
		return (0);
	}
	if (!init_philo(&data, argv, argc))
	{
		printf("%sError%s\n", RED, NC);
		return (0);
	}
	free(data.philo);
	return (0);
}