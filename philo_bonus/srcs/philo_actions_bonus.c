/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 01:49:09 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 17:13:49 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	safe_sleep(t_philo *philo, long duration)
{
	long	start_time;
	long	current_time;
	long	elapsed;

	get_time_ms(&start_time);
	while (1)
	{
		get_time_ms(&current_time);
		elapsed = current_time - start_time;
		check_death_during_sleep(philo, current_time);
		if (elapsed >= duration)
			break ;
		if (duration - elapsed > 10)
			usleep(5000);
		else
			usleep(1000);
	}
}

void	handle_single_philo_eat(t_philo *philo)
{
	safe_sleep(philo, philo->data->time_to_die + 10);
	sem_post(philo->data->forks);
	sem_post(philo->data->eaters_sem);
}

void	philo_eat(t_philo *philo)
{
	sem_wait(philo->data->eaters_sem);
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	if (philo->data->nb_philos == 1)
	{
		handle_single_philo_eat(philo);
		return ;
	}
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	sem_wait(philo->meal_time_sem);
	get_time_ms(&philo->last_meal_time);
	sem_post(philo->meal_time_sem);
	print_status(philo, "is eating");
	philo->meals_eaten++;
	safe_sleep(philo, philo->data->time_to_eat);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	sem_post(philo->data->eaters_sem);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	safe_sleep(philo, philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_status(philo, "is thinking");
	usleep(500);
}
