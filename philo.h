/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:14:14 by atifany           #+#    #+#             */
/*   Updated: 2022/03/21 15:14:16 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// Libs included
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

// General macros
# define ULL unsigned long long
# define INT_MAX 2147483647
# define INT_MIN -2147483648
# define TRUE 1
# define FALSE 0

// Global structure
typedef struct s_philo
{
	int				times_eaten;
	pthread_mutex_t	timer;
	long long		last_meal;
}	t_philo;

typedef struct s_data
{
	int				philososphers_hungry;
	char			dead;
	int				number_of_philosophers;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				times_each_philosopher_must_eat;
	long long		sim_start;
	t_philo			*philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	log_queue;
	pthread_mutex_t	get_time;
}	t_data;

typedef struct s_point
{
	int			num;
	t_data		*data;
}	t_point;

// Philosopher
char		init_philo(t_data *data, char **args, int argc);
void		*philosopher(void *arg);

// Philosopher actions
char		_think(t_data *data, int my_num);
char		_eat(t_data *data, int my_num, int left_fork, int right_fork);
char		_sleep(t_data *data, int my_num, int left_fork, int right_fork);

// Philosopher utils
void		*timer_to_die(void *arg);
long long	cur_time(t_data *data);
void		*count_to_death(void *arg);

// Utils
long long	ft_atoi(char *n);
void		unlock(pthread_mutex_t *lock_1, pthread_mutex_t *lock_2, pthread_mutex_t *lock_3);

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