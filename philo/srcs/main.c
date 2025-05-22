/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:44:56 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/22 20:52:52 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Initialize simulation resources */
static bool	init_simulation(t_data *data, t_philo **philos,
		pthread_mutex_t **forks)
{
	if (!init_mutexes(data, forks))
	{
		return (ft_error("error: failed to initialize mutexes",
				INTERNAL_ERROR) && false);
	}
	if (!init_philosophers(data, philos, *forks))
	{
		cleanup(data, *philos, *forks);
		return (ft_error("error: failed to initialize philosophers",
				INTERNAL_ERROR) && false);
	}
	return (true);
}

/* Run the simulation */
static int	run_simulation(t_data *data, t_philo *philos,
		pthread_mutex_t *forks)
{
	if (!start_simulation(data, philos))
	{
		cleanup(data, philos, forks);
		return (ft_error("error: failed to start simulation", INTERNAL_ERROR));
	}
	cleanup(data, philos, forks);
	return (OK);
}

int	main(int argc, char **argv)
{
	t_data			data;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	memset(&data, 0, sizeof(t_data));
	philos = NULL;
	forks = NULL;
	if (!parse_args(argc, argv, &data))
		return (ARG_ERROR);
	if (!init_simulation(&data, &philos, &forks))
		return (INTERNAL_ERROR);
	return (run_simulation(&data, philos, forks));
}
