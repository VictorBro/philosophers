/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:44:46 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 19:39:20 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <limits.h>

/* Parse arguments and validate values */
static bool	validate_args(int argc, t_data *data)
{
	if (data->num_philos <= 0)
		return (ft_error("error: number_of_philosophers must be positive "
				"and less than MAX_INT", ARG_ERROR) && false);
	if (data->time_to_die <= 0)
		return (ft_error("error: time_to_die must be positive "
				"and less than MAX_INT", ARG_ERROR) && false);
	if (data->time_to_eat <= 0)
		return (ft_error("error: time_to_eat must be positive "
				"and less than MAX_INT", ARG_ERROR) && false);
	if (data->time_to_sleep <= 0)
		return (ft_error("error: time_to_sleep must be positive "
				"and less than MAX_INT", ARG_ERROR) && false);
	if (argc == 6 && data->must_eat_count <= 0)
	{
		return (ft_error("error: meals count must be positive "
				"and less than MAX_INT", ARG_ERROR) && false);
	}
	return (true);
}

/* Display usage message */
static void	display_usage(char **argv)
{
	printf("Usage: %s <number_of_philosophers> <time_to_die> ", argv[0]);
	printf("<time_to_eat> <time_to_sleep> ");
	printf("[number_of_times_each_philosopher_must_eat]\n");
}

/* Skip whitespace and get sign */
static int	skip_space_get_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	while (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\n'
		|| str[*i] == '\v' || str[*i] == '\f' || str[*i] == '\r')
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

/* Extract integer from string */
static int	parse_int(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	i = 0;
	sign = skip_space_get_sign(str, &i);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if ((sign == 1 && result > INT_MAX)
			|| (sign == -1 && result * sign < INT_MIN))
			return (0);
		i++;
	}
	if (str[i] != '\0')
		return (0);
	return ((int)(result * sign));
}

/* Parse arguments */
bool	parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		display_usage(argv);
		return (false);
	}
	data->num_philos = parse_int(argv[1]);
	data->time_to_die = parse_int(argv[2]);
	data->time_to_eat = parse_int(argv[3]);
	data->time_to_sleep = parse_int(argv[4]);
	if (argc == 6)
		data->must_eat_count = parse_int(argv[5]);
	else
		data->must_eat_count = -1;
	data->someone_died = false;
	data->all_ate = 0;
	return (validate_args(argc, data));
}
