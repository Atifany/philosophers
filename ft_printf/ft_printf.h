/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atifany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 18:09:00 by atifany           #+#    #+#             */
/*   Updated: 2021/11/25 18:09:00 by atifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <unistd.h>

# define ULL unsigned long long

typedef struct s_formats {
	char	is_hesh;
	char	is_plus;
	char	is_space;
	ULL		width;
	ULL		precision;
	char	is_dot;
	char	is_minus;
	char	is_zero;
	int		ret;
}	t_formats;

//gen:
int		ft_printf(const char *format, ...);
//utils:
int		_ft_atoi(char *str);
size_t	_ft_strlen(const char *s);
void	_ft_bzero(void *s, size_t n);
int		read_number(const char **format);
void	clear_form(t_formats *form, char is_first_time);
void	_ft_swap(t_formats *form, size_t *ans_len, ULL *precision);
//printers:
void	ft_putstr(char *s, t_formats *form);
void	ft_putnbr(long long n, t_formats *form);
void	print_width(t_formats *form, size_t ans_len);
void	ft_putchar(char ch, t_formats *form, char is_formated);
void	ft_print_in_hex(ULL n, char is_upper, t_formats *form, char is_ptr);

#endif
