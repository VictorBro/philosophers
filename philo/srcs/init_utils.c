/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:29:00 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/01 12:08:33 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philo(t_data *philo_data)
{
	unsigned int	i;
	unsigned long	now;

	if (current_time_ms(&now) != OK)
		return (INTERNAL_ERROR);
	i = 0;
	while (i < philo_data->philos_nbr)
	{
		philo_data->philos[i].idx = i;
		philo_data->philos[i].meals_eaten = 0;
		philo_data->philos[i].dead = FALSE;
		philo_data->philos[i].data = philo_data;
		philo_data->philos[i].deadline = now + philo_data->time2die;
		i++;
	}
	return (OK);
}

int	alloc_data(t_data *philo_data)
{
	philo_data->philos = ft_calloc(sizeof(t_philo), philo_data->philos_nbr);
	if (!philo_data->philos)
		return (ft_error("error: philos allocation failed", ALLOC_ERROR));
	philo_data->forks = ft_calloc(sizeof(t_mtx), philo_data->philos_nbr);
	if (!philo_data->forks)
	{
		free(philo_data->philos);
		philo_data->philos = NULL;
		return (ft_error("error: forks allocation failed", ALLOC_ERROR));
	}
	return (init_philo(philo_data));
}

int	init_fork_mutexes(t_data *philo_data)
{
	unsigned int	i;
	int				res;

	i = 0;
	while (i < philo_data->philos_nbr)
	{
		res = mutex_op(&philo_data->forks[i], INIT);
		if (res != OK)
		{
			while (i > 0)
			{
				i--;
				mutex_op(&philo_data->forks[i], DESTROY);
			}
			return (res);
		}
		i++;
	}
	return (OK);
}

int	init_philo_mutexes(t_philo *philo)
{
	int	res;

	res = mutex_op(&philo->lock_meals_eaten, INIT);
	if (res != OK)
		return (res);
	res = mutex_op(&philo->lock_dead, INIT);
	if (res != OK)
	{
		mutex_op(&philo->lock_meals_eaten, DESTROY);
		return (res);
	}
	return (OK);
}

int	init_data(t_data *philo_data, int argc, char *argv[])
{
	if (ft_atou(argv[1], &philo_data->philos_nbr) != OK
		|| philo_data->philos_nbr < 1)
		return (ft_error("error: number_of_philosophers invalid", ARG_ERROR));
	if (ft_atou(argv[2], &philo_data->time2die) != OK)
		return (ft_error("error: time_to_die invalid", ARG_ERROR));
	if (ft_atou(argv[3], &philo_data->time2eat) != OK)
		return (ft_error("error: time_to_eat invalid", ARG_ERROR));
	if (ft_atou(argv[4], &philo_data->time2sleep) != OK)
		return (ft_error("error: time_to_sleep invalid", ARG_ERROR));
	philo_data->meals_isset = FALSE;
	if (argc == 6)
	{
		if (ft_atou(argv[5], &philo_data->meals_nbr) != OK)
			return (ft_error(
					"error: number_of_times_each_philosopher_must_eat invalid",
					ARG_ERROR));
		philo_data->meals_isset = TRUE;
	}
	return (alloc_data(philo_data));
}
