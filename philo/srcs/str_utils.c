/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:15:42 by vbronov           #+#    #+#             */
/*   Updated: 2025/04/21 01:17:20 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f');
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atou(char *str, unsigned int *num)
{
	*num = 0;
	while (*str && ft_isspace(*str))
		str++;
	if (!ft_isdigit(*str))
		return (ARG_ERROR);
	while (*str && ft_isdigit(*str))
	{
		if (*num > UINT_MAX / 10
			|| (*num == UINT_MAX / 10 && *str - '0' > UINT_MAX % 10))
			return (ARG_ERROR);
		*num = *num * 10 + *str - '0';
		str++;
	}
	while (*str && ft_isspace(*str))
		str++;
	if (*str)
		return (ARG_ERROR);
	return (OK);
}
