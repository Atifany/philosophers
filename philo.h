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
typedef struct s_data
{
	int				number_of_philosophers;
	ULL				time_to_die;
	ULL				time_to_eat;
	ULL				time_to_sleep;
	ULL				number_of_times_each_philosopher_must_eat;
	long long		sim_start;
	char			*is_dead;
	pthread_mutex_t	*forks;
}	t_data;

typedef struct s_point
{
	int			num;
	t_data		*data;
}	t_point;

// Utils
ULL	ft_atoi(char *n);

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