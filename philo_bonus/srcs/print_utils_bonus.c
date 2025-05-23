/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:49:09 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 16:33:35 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	ft_error(const char *msg, int ret_val)
{
	if (msg)
		printf("Error: %s\n", msg);
	return (ret_val);
}

void	print_status(t_philo *philo, char *status)
{
	long	current_time;

	sem_wait(philo->data->write_sem);
	if (get_time_ms(&current_time) != OK)
	{
		sem_post(philo->data->write_sem);
		return ;
	}
	printf("%ld %d %s\n", current_time, philo->id, status);
	sem_post(philo->data->write_sem);
}
