/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_routines.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:44:01 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/22 22:36:30 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Handle philosopher thinking and waiting */
static void	handle_thinking(t_philo *philo)
{
	print_state(philo->data, philo->id, "is thinking");
	if (philo->id % 2 == 0 && philo->data->num_philos % 2 == 1)
		precise_sleep(philo->data->time_to_eat * 0.3, philo->data);
	else
		precise_sleep(1, philo->data);
}

/* Philosopher routine */
void	*philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	print_state(philo->data, philo->id, "is thinking");
	if (philo->id % 2 == 0)
		precise_sleep(philo->data->time_to_eat * 0.5, philo->data);
	while (!should_stop(philo->data))
	{
		if (!take_forks(philo))
			continue ;
		eat(philo);
		release_forks(philo);
		if (should_stop(philo->data))
			break ;
		print_state(philo->data, philo->id, "is sleeping");
		precise_sleep(philo->data->time_to_sleep, philo->data);
		handle_thinking(philo);
	}
	return (NULL);
}
