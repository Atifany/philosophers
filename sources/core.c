/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:14:25 by atifany           #+#    #+#             */
/*   Updated: 2022/03/21 15:14:26 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*philosopher(void *arg)
{
	int			my_num;
	t_point		arg_tmp;
	t_data		*data;
	pthread_t	timer;

	arg_tmp = (*(t_point *)arg);
	data = arg_tmp.data;
	my_num = arg_tmp.num + 1;
	pthread_create(&timer, NULL, timer_to_die, arg);
	data->philo[my_num - 1].last_meal = cur_time(data);
	pthread_mutex_init(&data->philo[my_num - 1].timer, NULL);
	return (philo_life_cycle(arg, data, my_num));
}

int	main(int argc, char **argv)
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
	init_philo(&data, argv, argc);
	free(data.philo);
	free(data.forks);
	return (0);
}
