/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:45:25 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/19 02:03:29 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Set up fork order to prevent deadlocks */
static void	fork_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	if (philo->id % 2 == 0)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
}

/* Take first fork */
static bool	take_first_fork(t_philo *philo, pthread_mutex_t *first_fork)
{
	int	status;

	status = pthread_mutex_lock(first_fork);
	if (status != 0)
	{
		ft_error("error: failed to lock fork mutex", status);
		return (false);
	}
	print_state(philo->data, philo->id, "has taken a fork");
	return (true);
}

/* Handle single philosopher case */
static bool	handle_one_philo(t_philo *philo)
{
	int	status;

	status = pthread_mutex_lock(philo->left_fork);
	if (status != 0)
	{
		ft_error("error: failed to lock fork mutex", status);
		return (false);
	}
	print_state(philo->data, philo->id, "has taken a fork");
	while (!should_stop(philo->data))
		usleep(1000);
	pthread_mutex_unlock(philo->left_fork);
	return (false);
}

/* Take forks with deadlock prevention strategy */
bool	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	int				status;

	if (philo->data->num_philos == 1)
		return (handle_one_philo(philo));
	fork_order(philo, &first_fork, &second_fork);
	if (!take_first_fork(philo, first_fork))
		return (false);
	status = pthread_mutex_lock(second_fork);
	if (status != 0)
	{
		ft_error("error: failed to lock fork mutex", status);
		pthread_mutex_unlock(first_fork);
		return (false);
	}
	print_state(philo->data, philo->id, "has taken a fork");
	return (true);
}

/* Release forks after eating */
void	release_forks(t_philo *philo)
{
	int	status;

	if (philo->id % 2 == 0)
	{
		status = pthread_mutex_unlock(philo->right_fork);
		if (status != 0)
			ft_error("error: failed to unlock fork mutex", status);
		status = pthread_mutex_unlock(philo->left_fork);
		if (status != 0)
			ft_error("error: failed to unlock fork mutex", status);
	}
	else
	{
		status = pthread_mutex_unlock(philo->left_fork);
		if (status != 0)
			ft_error("error: failed to unlock fork mutex", status);
		status = pthread_mutex_unlock(philo->right_fork);
		if (status != 0)
			ft_error("error: failed to unlock fork mutex", status);
	}
}
