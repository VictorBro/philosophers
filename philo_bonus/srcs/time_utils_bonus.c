/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:49:09 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 16:18:11 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

/* Get current timestamp in milliseconds */
int	get_time_ms(long *current_time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != OK)
		return (ft_error("gettimeofday() failed", errno));
	*current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (OK);
}

/* Sleep for precise milliseconds */
void	precise_sleep(long milliseconds)
{
	long	start;
	long	current;
	long	elapsed;

	if (get_time_ms(&start) != OK)
		return ;
	while (TRUE)
	{
		if (get_time_ms(&current) != OK)
			return ;
		elapsed = current - start;
		if (elapsed >= milliseconds)
			break ;
		usleep(500);
	}
}
