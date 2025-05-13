/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 23:31:17 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/01 18:02:21 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die ", argv[0]);
		printf("time_to_eat time_to_sleep ");
		printf("[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	memset(&data, 0, sizeof(t_data));
	if (init_data(&data, argc, argv) != 0)
		return (1);
	if (start_simulation(&data) != 0)
	{
		end_simulation(&data);
		return (1);
	}
	end_simulation(&data);
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	// Unlink any existing semaphores with the same names
	cleanup_semaphores();
	
	memset(data, 0, sizeof(t_data));
	
	// Parse arguments
	data->nb_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	
	if (argc == 6)
		data->nb_times_to_eat = ft_atoi(argv[5]);
	else
		data->nb_times_to_eat = -1;

	// Validate arguments
	if (data->nb_philos <= 0 || data->time_to_die <= 0 || 
		data->time_to_eat <= 0 || data->time_to_sleep <= 0 || 
		(argc == 6 && data->nb_times_to_eat <= 0))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}

	// Allocate memory for philosopher PIDs
	data->pids = malloc(sizeof(pid_t) * data->nb_philos);
	if (!data->pids)
	{
		printf("Error: Memory allocation failed\n");
		return (1);
	}

	// Initialize semaphores
	data->forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 0644, data->nb_philos);
	data->write_sem = sem_open(SEM_WRITE, O_CREAT | O_EXCL, 0644, 1);
	data->dead_sem = sem_open(SEM_DEAD, O_CREAT | O_EXCL, 0644, 0);
	data->meal_sem = sem_open(SEM_MEALS, O_CREAT | O_EXCL, 0644, 0);
	// Create a semaphore to prevent deadlock - allow only N-1 philosophers at the table
	data->deadlock_sem = sem_open(SEM_DEADLOCK, O_CREAT | O_EXCL, 0644, data->nb_philos - 1);

	if (data->forks == SEM_FAILED || data->write_sem == SEM_FAILED || 
		data->dead_sem == SEM_FAILED || data->meal_sem == SEM_FAILED ||
		data->deadlock_sem == SEM_FAILED)
	{
		printf("Error: Semaphore initialization failed\n");
		return (1);
	}

	return (0);
}

int	start_simulation(t_data *data)
{
	int	i;
	int	status;
	int	finished_eating;

	data->start_time = get_time();
	if (data->start_time == -1)
		return (1);

	// Create philosopher processes
	i = 0;
	while (i < data->nb_philos)
	{
		data->pids[i] = fork();
		if (data->pids[i] == -1)
		{
			printf("Error: Fork failed\n");
			return (1);
		}
		else if (data->pids[i] == 0) // Child process
		{
			philosopher_routine(data, i + 1);
			exit(0);
		}
		i++;
		usleep(100); // Small delay to avoid race conditions
	}

	// Wait for simulation to end
	if (data->nb_times_to_eat > 0)
	{
		// Wait for all philosophers to finish eating
		finished_eating = 0;
		while (finished_eating < data->nb_philos)
		{
			sem_wait(data->meal_sem);
			finished_eating++;
		}
		// Kill all philosopher processes
		i = 0;
		while (i < data->nb_philos)
			kill(data->pids[i++], SIGTERM);
	}
	else
	{
		// Wait for a philosopher to die
		sem_wait(data->dead_sem);
		// Kill all philosopher processes
		i = 0;
		while (i < data->nb_philos)
			kill(data->pids[i++], SIGTERM);
	}

	// Wait for all child processes to terminate
	i = 0;
	while (i < data->nb_philos)
	{
		waitpid(data->pids[i], &status, 0);
		i++;
	}

	return (0);
}

void	end_simulation(t_data *data)
{
	// Close and unlink semaphores
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
	if (data->deadlock_sem != SEM_FAILED && data->deadlock_sem != NULL)
	{
		sem_close(data->deadlock_sem);
		sem_unlink(SEM_DEADLOCK);
	}

	// Free allocated memory
	if (data->pids)
		free(data->pids);
}

void	cleanup_semaphores(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_WRITE);
	sem_unlink(SEM_DEAD);
	sem_unlink(SEM_MEALS);
	sem_unlink(SEM_DEADLOCK);
}
