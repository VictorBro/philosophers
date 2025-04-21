/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:29:00 by vbronov           #+#    #+#             */
/*   Updated: 2025/04/21 02:23:36 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	alloc_data(t_data *philo_data)
{
	unsigned int	i;

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
	i = 0;
	while (i < philo_data->philos_nbr)
	{
		philo_data->philos[i].idx = i;
		philo_data->philos[i].meals_eaten = 0;
		philo_data->philos[i].dead = FALSE;
		philo_data->philos[i].data = philo_data;
		i++;
	}
	return (OK);
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

int	init_mutexes(t_data *philo_data)
{
	unsigned int	i;
	int				res;

	res = mutex_op(&philo_data->lock_print, INIT);
	if (res != OK)
		return (res);
	res = init_fork_mutexes(philo_data);
	if (res != OK)
		return (mutex_op(&philo_data->lock_print, DESTROY));
	i = 0;
	while (i < philo_data->philos_nbr)
	{
		res = init_philo_mutexes(&philo_data->philos[i]);
		if (res != OK)
			return (reverse_mutex_destroy(philo_data, i, res));
		i++;
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
	if (alloc_data(philo_data) != OK)
		return (ALLOC_ERROR);
	return (OK);
}
