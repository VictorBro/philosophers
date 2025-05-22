/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:45:41 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/22 22:11:27 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Check if philosopher has eaten enough meals */
static void	check_eat_count(t_philo *philo)
{
	int	status;

	if (philo->data->must_eat_count <= 0
		|| philo->eat_count < philo->data->must_eat_count)
		return ;
	status = pthread_mutex_lock(&philo->data->dead_mutex);
	if (status != 0)
	{
		ft_error("error: failed to lock dead mutex", status);
		return ;
	}
	if (philo->eat_count == philo->data->must_eat_count)
		philo->data->all_ate++;
	status = pthread_mutex_unlock(&philo->data->dead_mutex);
	if (status != 0)
		ft_error("error: failed to unlock dead mutex", status);
}

/* Eating function */
void	eat(t_philo *philo)
{
	int	status;

	print_state(philo->data, philo->id, "is eating");
	status = pthread_mutex_lock(&philo->meal_mutex);
	if (status != 0)
	{
		ft_error("error: failed to lock meal mutex", status);
		return ;
	}
	if (get_time(&philo->last_meal_time) != OK)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return ;
	}
	philo->eat_count++;
	status = pthread_mutex_unlock(&philo->meal_mutex);
	if (status != 0)
		ft_error("error: failed to unlock meal mutex", status);
	precise_sleep(philo->data->time_to_eat, philo->data);
	if (!should_stop(philo->data))
		check_eat_count(philo);
}
