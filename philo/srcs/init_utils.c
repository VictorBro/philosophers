/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:45:12 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/19 01:43:46 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Destroy initialized mutexes in case of error */
static void	destroy_fork_mutexes(pthread_mutex_t *forks, int count)
{
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		status = pthread_mutex_destroy(&forks[i]);
		if (status != 0)
			ft_error("error: failed to destroy mutex", status);
		i++;
	}
}

/* Initialize data mutexes */
static bool	init_data_mutexes(t_data *data)
{
	int	status;

	status = pthread_mutex_init(&data->write_mutex, NULL);
	if (status != 0)
		return (ft_error("error: failed to init write_mutex", status) && false);
	status = pthread_mutex_init(&data->dead_mutex, NULL);
	if (status != 0)
	{
		pthread_mutex_destroy(&data->write_mutex);
		return (ft_error("error: failed to init dead_mutex", status) && false);
	}
	return (true);
}

/* Initialize fork mutexes */
static bool	init_fork_mutexes(t_data *data, pthread_mutex_t **forks)
{
	int	i;
	int	status;

	*forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!*forks)
		return (ft_error("error: fork allocation failed", ALLOC_ERROR)
			&& false);
	i = 0;
	while (i < data->num_philos)
	{
		status = pthread_mutex_init(&(*forks)[i], NULL);
		if (status != 0)
		{
			destroy_fork_mutexes(*forks, i);
			free(*forks);
			*forks = NULL;
			return (ft_error("error: failed to init fork mutex", status)
				&& false);
		}
		i++;
	}
	return (true);
}

/* Initialize all mutexes */
bool	init_mutexes(t_data *data, pthread_mutex_t **forks)
{
	if (!init_data_mutexes(data))
		return (false);
	if (!init_fork_mutexes(data, forks))
	{
		pthread_mutex_destroy(&data->write_mutex);
		pthread_mutex_destroy(&data->dead_mutex);
		return (false);
	}
	return (true);
}
