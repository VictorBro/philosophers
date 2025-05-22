/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:30:00 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/19 01:39:55 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Destroy initialized meal mutexes in case of error */
static void	destroy_meal_mutexes(t_philo *philos, int count)
{
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		status = pthread_mutex_destroy(&philos[i].meal_mutex);
		if (status != 0)
			ft_error("error: failed to destroy meal mutex", status);
		i++;
	}
}

/* Setup a philosopher's initial data */
static bool	setup_philosopher(t_philo *philo, t_data *data,
				pthread_mutex_t *forks, int i)
{
	int	status;

	philo->id = i + 1;
	philo->eat_count = 0;
	if (get_time(&philo->last_meal_time) != OK)
		return (false);
	philo->data = data;
	philo->left_fork = &forks[i];
	philo->right_fork = &forks[(i + 1) % data->num_philos];
	status = pthread_mutex_init(&philo->meal_mutex, NULL);
	if (status != 0)
		return (ft_error("error: failed to init meal mutex", status) && false);
	return (true);
}

/* Initialize philosophers */
bool	init_philosophers(t_data *data, t_philo **philos,
				pthread_mutex_t *forks)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!*philos)
	{
		return (ft_error("error: philos allocation failed",
				ALLOC_ERROR) && false);
	}
	i = 0;
	while (i < data->num_philos)
	{
		if (!setup_philosopher(&(*philos)[i], data, forks, i))
		{
			destroy_meal_mutexes(*philos, i);
			free(*philos);
			*philos = NULL;
			return (false);
		}
		i++;
	}
	return (true);
}
