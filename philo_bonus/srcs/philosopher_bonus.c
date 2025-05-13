/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:39:59 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/01 18:02:41 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	clean_child_exit(t_philo *philo, int status)
{
	// Close all semaphores to prevent memory leaks
	if (philo->data->forks != SEM_FAILED && philo->data->forks != NULL)
		sem_close(philo->data->forks);
	if (philo->data->write_sem != SEM_FAILED && philo->data->write_sem != NULL)
		sem_close(philo->data->write_sem);
	if (philo->data->dead_sem != SEM_FAILED && philo->data->dead_sem != NULL)
		sem_close(philo->data->dead_sem);
	if (philo->data->meal_sem != SEM_FAILED && philo->data->meal_sem != NULL)
		sem_close(philo->data->meal_sem);
	if (philo->data->deadlock_sem != SEM_FAILED && philo->data->deadlock_sem != NULL)
		sem_close(philo->data->deadlock_sem);
	
	exit(status);
}

void	philosopher_routine(t_data *data, int philo_id)
{
	t_philo	philo;

	memset(&philo, 0, sizeof(t_philo));
	philo.id = philo_id;
	philo.data = data;
	philo.last_meal_time = get_time();
	philo.meals_eaten = 0;

	// Create a monitor thread for this philosopher
	if (pthread_create(&philo.monitor_thread, NULL, monitor_routine, &philo) != 0)
	{
		printf("Error: Failed to create monitor thread\n");
		clean_child_exit(&philo, 1);
	}
	pthread_detach(philo.monitor_thread);

	// Philosopher life cycle
	while (1)
	{
		philo_eat(&philo);
		philo_sleep(&philo);
		philo_think(&philo);
		
		// If we have reached the required number of meals, exit
		if (philo.data->nb_times_to_eat > 0 && 
			philo.meals_eaten >= philo.data->nb_times_to_eat)
		{
			// Signal that this philosopher has finished eating
			sem_post(philo.data->meal_sem);
			clean_child_exit(&philo, 0);
		}
	}
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	long	current_time;

	philo = (t_philo *)arg;
	while (1)
	{
		current_time = get_time();
		// Check if philosopher has died
		if (current_time - philo->last_meal_time > philo->data->time_to_die)
		{
			sem_wait(philo->data->write_sem);
			printf("%ld %d died\n", 
				current_time - philo->data->start_time, philo->id);
			// Signal that a philosopher has died
			sem_post(philo->data->dead_sem);
			clean_child_exit(philo, 1);
		}
		
		// Exit check moved to main philosopher routine to prevent double signaling
		usleep(1000); // Sleep for 1ms to reduce CPU usage
	}
	return (NULL);
}

void	philo_eat(t_philo *philo)
{
	// Prevent deadlock by limiting number of philosophers that can eat
	sem_wait(philo->data->deadlock_sem);
	
	// Take two forks
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");

	// Eat
	print_status(philo, "is eating");
	philo->last_meal_time = get_time();
	precise_sleep(philo->data->time_to_eat);
	philo->meals_eaten++;

	// Put down forks
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	
	// Allow another philosopher to access forks
	sem_post(philo->data->deadlock_sem);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_sleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_status(philo, "is thinking");
}

void	print_status(t_philo *philo, char *status)
{
	long	current_time;

	sem_wait(philo->data->write_sem);
	current_time = get_time() - philo->data->start_time;
	printf("%ld %d %s\n", current_time, philo->id, status);
	sem_post(philo->data->write_sem);
}
