/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 22:43:00 by vbronov           #+#    #+#             */
/*   Updated: 2025/04/21 22:44:43 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutexes(t_data *philo_data)
{
	unsigned int	i;
	int				res;

	res = mutex_op(&philo_data->lock_print, INIT);
	if (res != OK)
		return (res);
	res = init_fork_mutexes(philo_data);
	if (res != OK)
		return (mutex_op(&philo_data->lock_print, DESTROY));
	i = 0;
	while (i < philo_data->philos_nbr)
	{
		res = init_philo_mutexes(&philo_data->philos[i]);
		if (res != OK)
			return (reverse_mutex_destroy(philo_data, i, res));
		i++;
	}
	return (OK);
}

int	reverse_mutex_destroy(t_data *philo_data, unsigned int i, int res)
{
	while (i > 0)
	{
		i--;
		mutex_op(&philo_data->philos[i].lock_meals_eaten, DESTROY);
		mutex_op(&philo_data->philos[i].lock_dead, DESTROY);
	}
	destroy_fork_mutexes(philo_data);
	mutex_op(&philo_data->lock_print, DESTROY);
	return (res);
}

int	mutex_op_res(int status, t_operation code)
{
	if (0 == status)
		return (OK);
	if (EINVAL == status && (LOCK == code || UNLOCK == code))
		return (ft_error("error: the value specified by mutex is invalid",
				EINVAL));
	else if (EINVAL == status && INIT == code)
		return (ft_error("error: init mutex, invalid arguments", EINVAL));
	else if (EDEADLK == status)
		return (ft_error("error: deadlock would occur if the "
				"thread blocked waiting for mutex.", EDEADLK));
	else if (ENOMEM == status)
		return (ft_error("error: memory allocation error", ENOMEM));
	else if (EBUSY == status)
		return (ft_error("error: mutex is locked", EBUSY));
	else
		return (ft_error("error: unknown error", status));
}

int	mutex_op(t_mtx *mutex, t_operation code)
{
	if (LOCK == code)
		return (mutex_op_res(pthread_mutex_lock(mutex), code) != OK);
	else if (UNLOCK == code)
		return (mutex_op_res(pthread_mutex_unlock(mutex), code) != OK);
	else if (INIT == code)
		return (mutex_op_res(pthread_mutex_init(mutex, NULL),
				code) != OK);
	else if (DESTROY == code)
		return (mutex_op_res(pthread_mutex_destroy(mutex), code) != OK);
	else
		return (ft_error("error: wrong code for mutex_handle", INTERNAL_ERROR));
}
