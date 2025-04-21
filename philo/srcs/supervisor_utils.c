/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:39:46 by vbronov           #+#    #+#             */
/*   Updated: 2025/04/21 03:33:31 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	meals_supervisor(t_philo *philo_ptr, unsigned int *full_philos)
{
	t_data	*philo_data;

	philo_data = philo_ptr->data;
	if (philo_data->meals_isset)
	{
		mutex_op(&philo_ptr->lock_meals_eaten, LOCK);
		if (philo_ptr->meals_eaten >= philo_data->meals_nbr)
		{
			mutex_op(&philo_ptr->lock_meals_eaten, UNLOCK);
			(*full_philos)++;
		}
		else
			mutex_op(&philo_ptr->lock_meals_eaten, UNLOCK);
	}
}

int	dead_supervisor(t_data *philo_data, unsigned int i)
{
	t_philo	*philo_ptr;

	philo_ptr = &(philo_data->philos[i]);
	mutex_op(&philo_ptr->lock_dead, LOCK);
	if (philo_ptr->dead == TRUE)
	{
		mutex_op(&philo_ptr->lock_dead, UNLOCK);
		kill_other_philos(philo_data, i);
		return (TRUE);
	}
	mutex_op(&philo_ptr->lock_dead, UNLOCK);
	return (FALSE);
}

void	ft_supervisor(t_data *philo_data)
{
	unsigned int	i;
	t_philo			*philo_ptr;
	unsigned int	full_philos;

	while (TRUE)
	{
		full_philos = 0;
		i = 0;
		while (i < philo_data->philos_nbr)
		{
			philo_ptr = &(philo_data->philos[i]);
			if (dead_supervisor(philo_data, i) == TRUE)
				return ;
			meals_supervisor(philo_ptr, &full_philos);
			i++;
		}
		if (full_philos == philo_data->philos_nbr)
		{
			kill_other_philos(philo_data, 0);
			return ;
		}
		ft_msleep(2);
	}
}
