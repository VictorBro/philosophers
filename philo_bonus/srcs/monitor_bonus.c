/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 01:49:09 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 21:36:52 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	clean_child_exit(t_philo *philo, int status)
{
	if (philo->data->forks != SEM_FAILED && philo->data->forks != NULL)
		sem_close(philo->data->forks);
	if (philo->data->write_sem != SEM_FAILED && philo->data->write_sem != NULL)
		sem_close(philo->data->write_sem);
	if (philo->data->dead_sem != SEM_FAILED && philo->data->dead_sem != NULL)
		sem_close(philo->data->dead_sem);
	if (philo->data->meal_sem != SEM_FAILED && philo->data->meal_sem != NULL)
		sem_close(philo->data->meal_sem);
	if (philo->data->eaters_sem != SEM_FAILED
		&& philo->data->eaters_sem != NULL)
		sem_close(philo->data->eaters_sem);
	if (philo->meal_time_sem != SEM_FAILED && philo->meal_time_sem != NULL)
		sem_close(philo->meal_time_sem);
	exit(status);
}

void	philo_check_death(t_philo *philo, long current_time, long time_since)
{
	if (time_since > philo->data->time_to_die)
	{
		sem_post(philo->data->dead_sem);
		sem_wait(philo->data->write_sem);
		printf("%ld %d died\n", current_time, philo->id);
		clean_child_exit(philo, 1);
	}
}

void	calculate_sleep_time(long time_remaining, long *sleep_time)
{
	if (time_remaining < 10)
		*sleep_time = 1;
	else if (time_remaining < 50)
		*sleep_time = 2;
	else
		*sleep_time = 5;
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	long	current_time;
	long	time_since_last_meal;
	long	time_remaining;
	long	sleep_time;

	philo = (t_philo *)arg;
	while (1)
	{
		if (get_time_ms(&current_time) != OK)
			continue ;
		sem_wait(philo->meal_time_sem);
		time_since_last_meal = current_time - philo->last_meal_time;
		sem_post(philo->meal_time_sem);
		philo_check_death(philo, current_time, time_since_last_meal);
		time_remaining = philo->data->time_to_die - time_since_last_meal;
		calculate_sleep_time(time_remaining, &sleep_time);
		precise_sleep(sleep_time);
	}
	return (NULL);
}
