/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:39:46 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/01 14:16:41 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	meals_supervisor(t_philo *philo_ptr, unsigned int *finished_philos)
{
	t_data	*philo_data;

	philo_data = philo_ptr->data;
	if (philo_data->meals_isset)
	{
		mutex_op(&philo_ptr->lock_meals_eaten, LOCK);
		if (philo_ptr->meals_eaten >= philo_data->meals_nbr)
		{
			mutex_op(&philo_ptr->lock_meals_eaten, UNLOCK);
			(*finished_philos)++;
		}
		else
			mutex_op(&philo_ptr->lock_meals_eaten, UNLOCK);
	}
}

int	dead_supervisor(t_data *philo_data, unsigned int i)
{
	t_philo	*philo_ptr;

	philo_ptr = &(philo_data->philos[i]);
	if (is_dead(philo_ptr) == TRUE)
	{
		kill_other_philos(philo_data, i);
		return (TRUE);
	}
	return (FALSE);
}

void	ft_supervisor(t_data *philo_data)
{
	unsigned int	i;
	t_philo			*philo_ptr;
	unsigned int	finished_philos;

	while (TRUE)
	{
		finished_philos = 0;
		i = 0;
		while (i < philo_data->philos_nbr)
		{
			philo_ptr = &(philo_data->philos[i]);
			if (dead_supervisor(philo_data, i) == TRUE)
				return ;
			meals_supervisor(philo_ptr, &finished_philos);
			i++;
		}
		if (finished_philos == philo_data->philos_nbr)
		{
			kill_other_philos(philo_data, 0);
			return ;
		}
		ft_msleep(2);
	}
}
