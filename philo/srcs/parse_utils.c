/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:44:46 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/19 02:18:56 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Parse arguments and validate values */
static bool	validate_args(int argc, t_data *data)
{
	if (data->num_philos <= 0)
		return (ft_error("error: number_of_philosophers must be positive",
				ARG_ERROR) && false);
	if (data->time_to_die <= 0)
		return (ft_error("error: time_to_die must be positive",
				ARG_ERROR) && false);
	if (data->time_to_eat <= 0)
		return (ft_error("error: time_to_eat must be positive",
				ARG_ERROR) && false);
	if (data->time_to_sleep <= 0)
		return (ft_error("error: time_to_sleep must be positive",
				ARG_ERROR) && false);
	if (argc == 6 && data->must_eat_count <= 0)
	{
		return (ft_error("error: meals count must be positive",
				ARG_ERROR) && false);
	}
	return (true);
}

/* Display usage message */
static void	display_usage(void)
{
	ft_error("usage: ./philo <number_of_philosophers>", ARG_ERROR);
	ft_error(" <time_to_die> <time_to_eat> <time_to_sleep>", ARG_ERROR);
	ft_error(" [number_of_times_each_must_eat]", ARG_ERROR);
}

/* Extract integer from string */
static int	parse_int(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

/* Parse arguments */
bool	parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		display_usage();
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
