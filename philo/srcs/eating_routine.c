/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:39:28 by vbronov           #+#    #+#             */
/*   Updated: 2025/04/21 02:23:36 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	leave_forks(t_philo *philo)
{
	unsigned int	right_fork;
	unsigned int	left_fork;

	left_fork = philo->idx;
	right_fork = (philo->idx + 1) % philo->data->philos_nbr;
	if ((philo->idx % 2) == 0)
	{
		mutex_op(&philo->data->forks[right_fork], UNLOCK);
		mutex_op(&philo->data->forks[left_fork], UNLOCK);
	}
	else
	{
		mutex_op(&philo->data->forks[left_fork], UNLOCK);
		mutex_op(&philo->data->forks[right_fork], UNLOCK);
	}
}

int	eat(t_philo *philo)
{
	if (is_dead(philo) == TRUE)
		return (DIED_ERROR);
	if (take_forks(philo) != OK)
		return (DIED_ERROR);
	print_action(philo, EAT);
	ft_msleep(philo->data->time2eat);
	leave_forks(philo);
	if (update_deadline(philo) != OK)
		return (DIED_ERROR);
	mutex_op(&philo->lock_meals_eaten, LOCK);
	if (philo->data->meals_isset)
	{
		if (philo->meals_eaten < philo->data->meals_nbr)
			philo->meals_eaten++;
	}
	mutex_op(&philo->lock_meals_eaten, UNLOCK);
	return (OK);
}

int	take_forks_helper(t_philo *philo, unsigned int first_fork,
	unsigned int second_fork)
{
	mutex_op(&philo->data->forks[first_fork], LOCK);
	if (is_dead(philo) == TRUE)
	{
		mutex_op(&philo->data->forks[first_fork], UNLOCK);
		return (DIED_ERROR);
	}
	print_action(philo, FORK);
	if (first_fork == second_fork)
	{
		while (is_dead(philo) == FALSE)
			ft_msleep(5);
		mutex_op(&philo->data->forks[first_fork], UNLOCK);
		return (DIED_ERROR);
	}
	mutex_op(&philo->data->forks[second_fork], LOCK);
	if (is_dead(philo) == TRUE)
	{
		mutex_op(&philo->data->forks[second_fork], UNLOCK);
		mutex_op(&philo->data->forks[first_fork], UNLOCK);
		return (DIED_ERROR);
	}
	print_action(philo, FORK);
	return (OK);
}

int	take_forks(t_philo *philo)
{
	unsigned int	right_fork;
	unsigned int	left_fork;

	left_fork = philo->idx;
	right_fork = (philo->idx + 1) % philo->data->philos_nbr;
	if (philo->idx % 2 == 0)
	{
		if (take_forks_helper(philo, left_fork, right_fork) != OK)
			return (DIED_ERROR);
	}
	else
	{
		if (take_forks_helper(philo, right_fork, left_fork) != OK)
			return (DIED_ERROR);
	}
	return (OK);
}
