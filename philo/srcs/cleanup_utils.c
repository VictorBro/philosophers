/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:00:00 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/19 01:39:55 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Clean up philosopher related resources */
void	cleanup_philosophers(t_data *data, t_philo *philos)
{
	int	i;
	int	status;

	if (philos)
	{
		i = 0;
		while (i < data->num_philos)
		{
			status = pthread_mutex_destroy(&philos[i].meal_mutex);
			if (status != 0)
				ft_error("error: failed to destroy meal mutex", status);
			i++;
		}
		free(philos);
	}
}

/* Clean up fork mutexes */
void	cleanup_forks(t_data *data, pthread_mutex_t *forks)
{
	int	i;
	int	status;

	if (forks)
	{
		i = 0;
		while (i < data->num_philos)
		{
			status = pthread_mutex_destroy(&forks[i]);
			if (status != 0)
				ft_error("error: failed to destroy fork mutex", status);
			i++;
		}
		free(forks);
	}
}

/* Clean up mutexes and allocated memory */
void	cleanup(t_data *data, t_philo *philos, pthread_mutex_t *forks)
{
	int	status;

	cleanup_philosophers(data, philos);
	cleanup_forks(data, forks);
	status = pthread_mutex_destroy(&data->write_mutex);
	if (status != 0)
		ft_error("error: failed to destroy write mutex", status);
	status = pthread_mutex_destroy(&data->dead_mutex);
	if (status != 0)
		ft_error("error: failed to destroy dead mutex", status);
}
