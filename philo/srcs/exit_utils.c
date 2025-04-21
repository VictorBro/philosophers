/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:39:36 by vbronov           #+#    #+#             */
/*   Updated: 2025/04/21 12:23:13 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_data(t_data *philo_data)
{
	if (philo_data->forks)
	{
		free(philo_data->forks);
		philo_data->forks = NULL;
	}
	if (philo_data->philos)
	{
		free(philo_data->philos);
		philo_data->philos = NULL;
	}
}

void	destroy_fork_mutexes(t_data *philo_data)
{
	unsigned int	i;

	i = 0;
	while (i < philo_data->philos_nbr)
	{
		mutex_op(&philo_data->forks[i], DESTROY);
		i++;
	}
}

void	destroy_mutexes(t_data *philo_data)
{
	unsigned int	i;

	i = 0;
	while (i < philo_data->philos_nbr)
	{
		mutex_op(&philo_data->philos[i].lock_meals_eaten, DESTROY);
		mutex_op(&philo_data->philos[i].lock_dead, DESTROY);
		i++;
	}
	destroy_fork_mutexes(philo_data);
	mutex_op(&philo_data->lock_print, DESTROY);
}

void	revert_threads(t_data *philo_data, unsigned int idx)
{
	unsigned int	i;
	t_philo			*philo_ptr;

	i = idx;
	while (i > 0)
	{
		--i;
		philo_ptr = &(philo_data->philos[i]);
		mutex_op(&philo_ptr->lock_dead, LOCK);
		philo_ptr->dead = TRUE;
		mutex_op(&philo_ptr->lock_dead, UNLOCK);
	}
	i = idx;
	while (i > 0)
	{
		--i;
		philo_ptr = &(philo_data->philos[i]);
		thread_op(&philo_ptr->thread_id, NULL, NULL, JOIN);
	}
}

void	kill_other_philos(t_data *philo_data, unsigned int idx)
{
	unsigned int	i;
	t_philo			*philo_ptr;
	unsigned long	time_died;

	i = 0;
	while (i < philo_data->philos_nbr)
	{
		philo_ptr = &(philo_data->philos[idx]);
		mutex_op(&philo_ptr->lock_dead, LOCK);
		if (philo_ptr->dead == TRUE)
		{
			time_died = philo_ptr->deadline;
			mutex_op(&philo_ptr->lock_dead, UNLOCK);
			print_dead_time(philo_data, time_died, idx);
		}
		else
		{
			philo_ptr->dead = TRUE;
			mutex_op(&philo_ptr->lock_dead, UNLOCK);
		}
		idx = (idx + 1) % philo_data->philos_nbr;
		i++;
	}
	join_threads(philo_data);
}
