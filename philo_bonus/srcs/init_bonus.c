/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 01:49:09 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 21:35:00 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	parse_arguments(t_data *data, int argc, char **argv)
{
	data->nb_philos = parse_int(argv[1]);
	data->time_to_die = parse_int(argv[2]);
	data->time_to_eat = parse_int(argv[3]);
	data->time_to_sleep = parse_int(argv[4]);
	if (argc == 6)
		data->nb_times_to_eat = parse_int(argv[5]);
	else
		data->nb_times_to_eat = -1;
}

int	validate_arguments(t_data *data, int argc)
{
	if (data->nb_philos <= 0)
		return (ft_error("Number of philosophers must be positive "
				"and less than MAX_INT", ARG_ERROR));
	if (data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (ft_error("Time values must be positive "
				"and less than MAX_INT", ARG_ERROR));
	if (argc == 6 && data->nb_times_to_eat <= 0)
		return (ft_error("Number of times to eat must be positive "
				"and less than MAX_INT", ARG_ERROR));
	return (OK);
}

int	init_semaphores(t_data *data)
{
	int	max_eaters;

	data->forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 0644, data->nb_philos);
	data->write_sem = sem_open(SEM_WRITE, O_CREAT | O_EXCL, 0644, 1);
	data->dead_sem = sem_open(SEM_DEAD, O_CREAT | O_EXCL, 0644, 0);
	data->meal_sem = sem_open(SEM_MEALS, O_CREAT | O_EXCL, 0644, 0);
	max_eaters = data->nb_philos;
	if (data->nb_philos > 1)
		max_eaters = data->nb_philos - 1;
	data->eaters_sem = sem_open(SEM_EATERS, O_CREAT | O_EXCL, 0644, max_eaters);
	cleanup_semaphores();
	if (data->forks == SEM_FAILED)
		return (ft_error("Forks semaphore init failed", SEMAPHORE_ERROR));
	if (data->write_sem == SEM_FAILED)
		return (ft_error("Write semaphore init failed", SEMAPHORE_ERROR));
	if (data->dead_sem == SEM_FAILED)
		return (ft_error("Dead semaphore init failed", SEMAPHORE_ERROR));
	if (data->meal_sem == SEM_FAILED)
		return (ft_error("Meal semaphore init failed", SEMAPHORE_ERROR));
	if (data->eaters_sem == SEM_FAILED)
		return (ft_error("Eaters semaphore init failed", SEMAPHORE_ERROR));
	return (OK);
}

int	init_data(t_data *data, int argc, char **argv)
{
	cleanup_semaphores();
	parse_arguments(data, argc, argv);
	if (validate_arguments(data, argc) != OK)
		return (ARG_ERROR);
	data->pids = malloc(sizeof(pid_t) * data->nb_philos);
	if (!data->pids)
		return (ft_error("Memory allocation failed", ALLOC_ERROR));
	return (init_semaphores(data));
}
