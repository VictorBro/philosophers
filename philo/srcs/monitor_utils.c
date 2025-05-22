/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:30:00 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/19 02:32:03 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Check if philosopher has starved */
static bool	check_philosopher_status(t_data *data, t_philo *philos, int i)
{
	long	current_time;
	long	time_since_last_meal;
	int		status;

	if (get_time(&current_time) != OK)
		return (false);
	status = pthread_mutex_lock(&philos[i].meal_mutex);
	if (status != 0)
	{
		ft_error("error: failed to lock meal mutex", status);
		return (true);
	}
	time_since_last_meal = current_time - philos[i].last_meal_time;
	status = pthread_mutex_unlock(&philos[i].meal_mutex);
	if (status != 0)
		ft_error("error: failed to unlock meal mutex", status);
	if (time_since_last_meal > data->time_to_die)
	{
		print_state(data, philos[i].id, "died");
		mark_death(data);
		return (false);
	}
	return (true);
}

/* Monitor philosophers for starvation */
bool	monitor_philosophers(t_data *data, t_philo *philos)
{
	int	i;

	while (!should_stop(data))
	{
		i = 0;
		while (i < data->num_philos && !should_stop(data))
		{
			if (!check_philosopher_status(data, philos, i))
				return (true);
			i++;
		}
		usleep(500);
	}
	return (true);
}
