/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 01:49:09 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 17:14:05 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	kill_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (kill(data->pids[i], SIGTERM) == -1)
			kill(data->pids[i], SIGKILL);
		i++;
	}
	usleep(1000);
}

void	wait_for_philosophers(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->nb_philos)
	{
		waitpid(data->pids[i], &status, 0);
		i++;
	}
}

void	cleanup_semaphores(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_WRITE);
	sem_unlink(SEM_DEAD);
	sem_unlink(SEM_MEALS);
	sem_unlink(SEM_EATERS);
}

void	close_semaphores(t_data *data)
{
	if (data->forks != SEM_FAILED && data->forks != NULL)
	{
		sem_close(data->forks);
		sem_unlink(SEM_FORKS);
	}
	if (data->write_sem != SEM_FAILED && data->write_sem != NULL)
	{
		sem_close(data->write_sem);
		sem_unlink(SEM_WRITE);
	}
	if (data->dead_sem != SEM_FAILED && data->dead_sem != NULL)
	{
		sem_close(data->dead_sem);
		sem_unlink(SEM_DEAD);
	}
	if (data->meal_sem != SEM_FAILED && data->meal_sem != NULL)
	{
		sem_close(data->meal_sem);
		sem_unlink(SEM_MEALS);
	}
}

void	end_simulation(t_data *data)
{
	close_semaphores(data);
	if (data->eaters_sem != SEM_FAILED && data->eaters_sem != NULL)
	{
		sem_close(data->eaters_sem);
		sem_unlink(SEM_EATERS);
	}
	if (data->pids)
		free(data->pids);
}
