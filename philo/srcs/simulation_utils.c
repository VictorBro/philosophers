/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:44:16 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/22 19:26:38 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Terminate all created threads in case of error */
static void	terminate_threads(t_philo *philos, int count)
{
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		status = pthread_detach(philos[i].thread);
		if (status != 0)
			ft_error("error: failed to detach thread", status);
		i++;
	}
}

/* Mark a philosopher as dead */
void	mark_death(t_data *data)
{
	int	status;

	status = pthread_mutex_lock(&data->dead_mutex);
	if (status != 0)
		ft_error("error: failed to lock dead mutex", status);
	data->someone_died = true;
	status = pthread_mutex_unlock(&data->dead_mutex);
	if (status != 0)
		ft_error("error: failed to unlock dead mutex", status);
}

/* Join all philosopher threads at end of simulation */
static bool	join_philosopher_threads(t_data *data, t_philo *philos)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->num_philos)
	{
		status = pthread_join(philos[i].thread, NULL);
		if (status != 0)
		{
			ft_error("error: failed to join thread", status);
			return (false);
		}
		i++;
	}
	return (true);
}

/* Create philosopher threads */
static bool	create_philosopher_threads(t_data *data, t_philo *philos)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->num_philos)
	{
		status = pthread_create(&philos[i].thread, NULL, philosopher,
				&philos[i]);
		if (status != 0)
		{
			ft_error("error: failed to create thread", status);
			terminate_threads(philos, i);
			return (false);
		}
		i++;
	}
	return (true);
}

/* Start threads for philosophers */
bool	start_simulation(t_data *data, t_philo *philos)
{
	if (!create_philosopher_threads(data, philos))
		return (false);
	if (!monitor_philosophers(data, philos))
		return (false);
	return (join_philosopher_threads(data, philos));
}
