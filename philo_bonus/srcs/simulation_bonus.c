/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   */
/*   Created: 2025/05/23 01:49:09 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 13:09:16 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	create_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		data->pids[i] = fork();
		if (data->pids[i] == -1)
			return (ft_error("Fork failed for philosopher process",
					FORK_ERROR));
		else if (data->pids[i] == 0)
		{
			philosopher_routine(data, i + 1);
			exit(0);
		}
		i++;
		precise_sleep(1);
	}
	return (OK);
}

void	*meal_monitor_routine(void *arg)
{
	t_data	*data;
	int		finished_eating;
	int		i;

	data = (t_data *)arg;
	finished_eating = 0;
	while (finished_eating < data->nb_philos)
	{
		sem_wait(data->meal_sem);
		finished_eating++;
	}
	i = 0;
	while (i < data->nb_philos)
	{
		kill(data->pids[i], SIGTERM);
		i++;
	}
	sem_post(data->dead_sem);
	return (NULL);
}

int	start_simulation(t_data *data)
{
	pthread_t	meal_monitor;

	if (create_philosophers(data) != OK)
		return (FORK_ERROR);
	if (data->nb_times_to_eat > 0)
	{
		if (pthread_create(&meal_monitor, NULL, meal_monitor_routine,
				data) != 0)
		{
			kill_philosophers(data);
			wait_for_philosophers(data);
			return (ft_error("Failed to create meal monitor thread", 1));
		}
		pthread_detach(meal_monitor);
	}
	sem_wait(data->dead_sem);
	kill_philosophers(data);
	wait_for_philosophers(data);
	return (OK);
}
