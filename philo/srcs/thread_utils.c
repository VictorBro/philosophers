/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:39:59 by vbronov           #+#    #+#             */
/*   Updated: 2025/04/21 22:43:21 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	thread_op_res(int status, t_operation code)
{
	if (0 == status)
		return (OK);
	if (EINVAL == status && JOIN == code)
		return (ft_error("error: thread is not a joinable thread", EINVAL));
	else if (EINVAL == status && DETACH == code)
		return (ft_error("error: thread is not a joinable thread", EINVAL));
	else if (EINVAL == status && INIT == code)
		return (ft_error("error: invalid attributes", EINVAL));
	else if (EINVAL == status && DETACH == code)
		return (ft_error("error: thread is not a joinable thread", EINVAL));
	else if (EINVAL == status && JOIN == code)
		return (ft_error("error: thread is not a joinable thread", EINVAL));
	else if (ENOMEM == status)
		return (ft_error("error: insufficient memory to create another thread",
				ENOMEM));
	else
		return (ft_error("error: unknown error", status));
}

int	thread_op(pthread_t *thread, void *(*start_routine)(void *),
	void *data, t_operation code)
{
	if (JOIN == code)
		return (thread_op_res(pthread_join(*thread, NULL), code) != OK);
	else if (DETACH == code)
		return (thread_op_res(pthread_detach(*thread), code) != OK);
	else if (INIT == code)
		return (thread_op_res(pthread_create(thread, NULL, start_routine,
					data), code) != OK);
	else
		return (ft_error("error: wrong code for thread_handle",
				INTERNAL_ERROR));
}
