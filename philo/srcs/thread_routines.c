/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_routines.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:44:01 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/19 01:39:55 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Handle philosopher thinking and waiting */
static void	handle_thinking(t_philo *philo)
{
	print_state(philo->data, philo->id, "is thinking");
	if (philo->id % 2 == 0 && philo->data->num_philos % 2 == 1)
		precise_sleep(philo->data->time_to_eat * 0.3);
	else
		precise_sleep(1);
}

/* Philosopher routine */
void	*philosopher(void *arg)
{
	t_philo	*philo;
	int		status;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
	{
		print_state(philo->data, philo->id, "is thinking");
		status = usleep(philo->data->time_to_eat * 500);
		if (status != 0)
			ft_error("error: usleep failed", status);
	}
	while (!should_stop(philo->data))
	{
		if (!take_forks(philo))
			continue ;
		eat(philo);
		release_forks(philo);
		print_state(philo->data, philo->id, "is sleeping");
		precise_sleep(philo->data->time_to_sleep);
		handle_thinking(philo);
	}
	return (NULL);
}
