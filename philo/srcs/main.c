/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 23:31:17 by vbronov           #+#    #+#             */
/*   Updated: 2025/04/21 03:23:31 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (update_deadline(philo) != OK)
		return (NULL);
	while (TRUE)
	{
		print_action(philo, THINK);
		if (eat(philo) != OK)
			break ;
		if (go_sleep(philo) != OK)
			break ;
		if (is_dead(philo) == TRUE)
			break ;
	}
	return (NULL);
}

int	create_philos(t_data *philo_data)
{
	unsigned int	i;
	int				res;
	t_philo			*philo_ptr;

	i = 0;
	while (i < philo_data->philos_nbr)
	{
		philo_ptr = &(philo_data->philos[i]);
		res = thread_op(&philo_ptr->thread_id, start_routine, philo_ptr, INIT);
		if (res != OK)
		{
			revert_threads(philo_data, i);
			return (res);
		}
		i++;
	}
	return (OK);
}

void	join_threads(t_data *philo_data)
{
	unsigned int	i;

	i = 0;
	while (i < philo_data->philos_nbr)
	{
		thread_op(&philo_data->philos[i].thread_id, NULL, NULL, JOIN);
		i++;
	}
}

static int	return_and_free(t_data *philo_data, int ret)
{
	destroy_mutexes(philo_data);
	free_data(philo_data);
	return (ret);
}

int	main(int argc, char *argv[])
{
	t_data	philo_data;
	int		res;

	if (argc < 5 || argc > 6)
		return (ft_error("usage: ./philo <number_of_philosophers> " \
		"<time_to_die> <time_to_eat> <time_to_sleep> " \
		"[number_of_times_each_philosopher_must_eat]", ARG_ERROR));
	res = init_data(&philo_data, argc, argv);
	if (res != OK)
		return (res);
	res = init_mutexes(&philo_data);
	if (res != OK)
		return (free_data(&philo_data), res);
	if (create_philos(&philo_data) != OK)
		return (return_and_free(&philo_data, INTERNAL_ERROR));
	ft_supervisor(&philo_data);
	return (return_and_free(&philo_data, OK));
}
