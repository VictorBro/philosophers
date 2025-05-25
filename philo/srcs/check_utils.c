/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:45:57 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/25 03:08:51 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Check if philosopher should stop (died or all ate enough) */
bool	should_stop(t_data *data)
{
	int		status;
	bool	ret;

	status = pthread_mutex_lock(&data->dead_mutex);
	if (status != 0)
	{
		ft_error("error: failed to lock dead mutex", status);
		return (true);
	}
	ret = data->someone_died
		|| (data->must_eat_count > 0 && data->all_ate >= data->num_philos);
	status = pthread_mutex_unlock(&data->dead_mutex);
	if (status != 0)
		ft_error("error: failed to unlock dead mutex", status);
	return (ret);
}
