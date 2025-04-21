/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:38:00 by vbronov           #+#    #+#             */
/*   Updated: 2025/04/21 02:23:36 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	time_print_helper(t_data *philo_data, unsigned long time,
	unsigned int idx)
{
	mutex_op(&philo_data->lock_print, LOCK);
	printf("%lu %u died\n", time, idx + 1);
	mutex_op(&philo_data->lock_print, UNLOCK);
}

int	ft_error(const char *msg, int ret_val)
{
	printf("%s\n", msg);
	return (ret_val);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*arr;

	arr = malloc(nmemb * size);
	if (!arr)
		return (NULL);
	memset(arr, 0, nmemb * size);
	return (arr);
}
