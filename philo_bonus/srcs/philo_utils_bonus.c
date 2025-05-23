/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 01:49:09 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 17:13:44 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	check_meals_and_exit(t_philo *philo)
{
	if (philo->data->nb_times_to_eat > 0
		&& philo->meals_eaten >= philo->data->nb_times_to_eat)
	{
		sem_post(philo->data->meal_sem);
		clean_child_exit(philo, 0);
	}
}

void	check_death_during_sleep(t_philo *philo, long current_time)
{
	long	time_since_last_meal;

	sem_wait(philo->meal_time_sem);
	time_since_last_meal = current_time - philo->last_meal_time;
	sem_post(philo->meal_time_sem);
	if (time_since_last_meal > philo->data->time_to_die)
	{
		sem_post(philo->data->dead_sem);
		sem_wait(philo->data->write_sem);
		printf("%ld %d died\n", current_time, philo->id);
		clean_child_exit(philo, 1);
	}
}
