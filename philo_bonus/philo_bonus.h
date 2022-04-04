/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:40:18 by atifany           #+#    #+#             */
/*   Updated: 2022/04/01 14:40:19 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

// Libs included
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <semaphore.h>
# include <sys/time.h>
# include <sys/errno.h>
# include <signal.h>

// General macros
# define ULL unsigned long long
# define INT_MAX 2147483647
# define INT_MIN -2147483648
# define TRUE 1
# define FALSE 0

// Global structures
typedef struct s_data
{
	int				times_each_philosopher_must_eat;
	int				philososphers_hungry;
	int				number_of_philosophers;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	long long		sim_start;
}	t_data;

// Data transfer structure
typedef struct s_transfer
{
	int				my_num;
	t_data			*data;
	pthread_mutex_t	eat_lock;
	char			is_dead;
	union
	{
		struct
		{
			sem_t	*sem_logs;
			sem_t	*sem_forks;
		}	t_sems;
		struct
		{
			int				times_eaten;
			long long		last_meal;
		}	t_philo;
	};
}	t_transfer;

// Philosopher
void	philosopher(t_transfer *info);
char	init_philo(t_data *data, char **args, int argc);

// Philosopher actions
char	_think(t_transfer *info);
char	_eat(t_transfer *info);
char	_sleep(t_transfer *info);

// Utils
long long	ft_atoi(char *n);
long long	cur_time(t_data *data);

// Colors
# define RED "\e[0;31m"
# define BLK "\e[0;30m"
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define BLU "\e[0;34m"
# define MAG "\e[0;35m"
# define CYN "\e[0;36m"
# define WHT "\e[0;37m"
# define NC "\e[0m"

#endif