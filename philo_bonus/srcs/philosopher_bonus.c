/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:10:23 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 21:59:30 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	init_philo(t_philo *philo, t_data *data, int philo_id)
{
	memset(philo, 0, sizeof(t_philo));
	philo->id = philo_id;
	philo->data = data;
	philo->meals_eaten = 0;
	create_sem_name(philo->meal_time_sem_name, SEM_MEAL_TIME, philo_id);
	sem_unlink(philo->meal_time_sem_name);
	philo->meal_time_sem = sem_open(philo->meal_time_sem_name,
			O_CREAT | O_EXCL, 0644, 1);
	sem_unlink(philo->meal_time_sem_name);
	if (philo->meal_time_sem == SEM_FAILED)
	{
		ft_error("Failed to create meal time semaphore", 1);
		clean_child_exit(philo, 1);
	}
	sem_wait(philo->meal_time_sem);
	get_time_ms(&philo->last_meal_time);
	sem_post(philo->meal_time_sem);
}

void	stagger_start(t_philo *philo)
{
	if (philo->id % 2 == 0)
		precise_sleep(philo->data->time_to_eat / 2);
	else if (philo->data->nb_philos > 1)
		precise_sleep(1);
}

void	handle_single_philo(t_philo *philo)
{
	print_status(philo, "has taken a fork");
	precise_sleep(philo->data->time_to_die + 10);
}

void	philosopher_routine(t_data *data, int philo_id)
{
	t_philo	philo;

	init_philo(&philo, data, philo_id);
	if (pthread_create(&philo.monitor_thread, NULL,
			monitor_routine, &philo) != 0)
	{
		ft_error("Failed to create monitor thread", 1);
		clean_child_exit(&philo, 1);
	}
	pthread_detach(philo.monitor_thread);
	print_status(&philo, "is thinking");
	stagger_start(&philo);
	if (data->nb_philos == 1)
	{
		handle_single_philo(&philo);
		return ;
	}
	while (1)
	{
		philo_eat(&philo);
		check_meals_and_exit(&philo);
		philo_sleep(&philo);
		philo_think(&philo);
	}
}
