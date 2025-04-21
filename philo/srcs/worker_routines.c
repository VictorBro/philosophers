/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_routines.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:40:11 by vbronov           #+#    #+#             */
/*   Updated: 2025/04/21 02:44:45 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philo *philo, t_action action)
{
	unsigned long	now;

	if (current_time_ms(&now) != OK)
		return ;
	mutex_op(&philo->data->lock_print, LOCK);
	if (action == FORK)
		printf("%lu %u has taken a fork\n", now, philo->idx + 1);
	else if (action == EAT)
		printf("%lu %u is eating\n", now, philo->idx + 1);
	else if (action == SLEEP)
		printf("%lu %u is sleeping\n", now, philo->idx + 1);
	else if (action == THINK)
		printf("%lu %u is thinking\n", now, philo->idx + 1);
	mutex_op(&philo->data->lock_print, UNLOCK);
}

int	is_dead(t_philo *philo)
{
	unsigned long	now;

	if (current_time_ms(&now) != OK)
		return (FALSE);
	mutex_op(&philo->lock_dead, LOCK);
	if (philo->dead == TRUE)
	{
		mutex_op(&philo->lock_dead, UNLOCK);
		return (TRUE);
	}
	if (now >= philo->deadline)
	{
		philo->deadline = now;
		philo->dead = TRUE;
		mutex_op(&philo->lock_dead, UNLOCK);
		return (TRUE);
	}
	mutex_op(&philo->lock_dead, UNLOCK);
	return (FALSE);
}

int	update_deadline(t_philo *philo)
{
	unsigned long	now;

	if (current_time_ms(&now) != OK)
	{
		mutex_op(&philo->lock_dead, LOCK);
		philo->dead = TRUE;
		mutex_op(&philo->lock_dead, UNLOCK);
		return (DIED_ERROR);
	}
	mutex_op(&philo->lock_dead, LOCK);
	philo->deadline = now + philo->data->time2die;
	mutex_op(&philo->lock_dead, UNLOCK);
	return (OK);
}

int	go_sleep(t_philo *philo)
{
	if (is_dead(philo) == TRUE)
		return (DIED_ERROR);
	print_action(philo, SLEEP);
	ft_msleep(philo->data->time2sleep);
	return (OK);
}
