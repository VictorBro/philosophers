/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:43:35 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/22 22:06:56 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Get current timestamp in milliseconds */
int	get_time(long *current_time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != OK)
		return (ft_error("error: gettimeofday() failed", errno));
	*current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (OK);
}

/* Sleep for precise milliseconds while checking if sim should stop */
void	precise_sleep(long ms, t_data *data)
{
	long	start;
	long	current;
	long	elapsed;

	if (get_time(&start) != OK)
		return ;
	while (TRUE)
	{
		if (should_stop(data))
			return ;
		if (get_time(&current) != OK)
			return ;
		elapsed = current - start;
		if (elapsed >= ms)
			break ;
		usleep(500);
	}
}
