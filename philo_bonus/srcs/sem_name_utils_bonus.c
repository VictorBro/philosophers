/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_name_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:10:23 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 21:44:25 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	add_digit_to_name(char *dest, int *i, int digit)
{
	dest[*i] = '0' + digit;
	(*i)++;
}

void	copy_prefix(char *dest, const char *prefix, int *i)
{
	while (prefix[*i])
	{
		dest[*i] = prefix[*i];
		(*i)++;
	}
}

void	add_id_to_name(char *dest, int philo_id, int *i)
{
	int		j;
	int		temp;
	int		digits;
	char	buf[12];

	if (philo_id == 0)
		dest[(*i)++] = '0';
	else
	{
		digits = 0;
		temp = philo_id;
		while (temp > 0)
		{
			buf[digits++] = '0' + (temp % 10);
			temp /= 10;
		}
		j = 0;
		while (j < digits)
			dest[(*i)++] = buf[digits - j++ - 1];
	}
}

void	create_sem_name(char *dest, const char *prefix, int philo_id)
{
	int	i;

	i = 0;
	copy_prefix(dest, prefix, &i);
	add_id_to_name(dest, philo_id, &i);
	dest[i] = '\0';
}
