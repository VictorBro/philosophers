/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:44:33 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/19 01:39:55 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Print the message if no philosopher has died */
static void	print_message(t_data *data, int id, char *state, long timestamp)
{
	if (!data->someone_died)
		printf("%ld %d %s\n", timestamp, id, state);
}

/* Get timestamp and lock write mutex */
static bool	prepare_print(t_data *data, long *timestamp)
{
	int	status;

	status = pthread_mutex_lock(&data->write_mutex);
	if (status != 0)
	{
		ft_error("error: failed to lock write mutex", status);
		return (false);
	}
	if (get_time(timestamp) != OK)
	{
		pthread_mutex_unlock(&data->write_mutex);
		return (false);
	}
	status = pthread_mutex_lock(&data->dead_mutex);
	if (status != 0)
	{
		ft_error("error: failed to lock dead mutex", status);
		pthread_mutex_unlock(&data->write_mutex);
		return (false);
	}
	return (true);
}

/* Print state changes with timestamp */
void	print_state(t_data *data, int id, char *state)
{
	long	timestamp;
	int		status;

	if (!prepare_print(data, &timestamp))
		return ;
	print_message(data, id, state, timestamp);
	status = pthread_mutex_unlock(&data->dead_mutex);
	if (status != 0)
		ft_error("error: failed to unlock dead mutex", status);
	status = pthread_mutex_unlock(&data->write_mutex);
	if (status != 0)
		ft_error("error: failed to unlock write mutex", status);
}

int	ft_error(const char *msg, int ret_val)
{
	printf("%s\n", msg);
	return (ret_val);
}
