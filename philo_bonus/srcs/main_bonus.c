/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 23:31:17 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 14:52:29 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		ret;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s <number_of_philosophers> <time_to_die> ", argv[0]);
		printf("<time_to_eat> <time_to_sleep> ");
		printf("[number_of_times_each_philosopher_must_eat]\n");
		return (ARG_ERROR);
	}
	memset(&data, 0, sizeof(t_data));
	ret = init_data(&data, argc, argv);
	if (ret != OK)
	{
		end_simulation(&data);
		return (ret);
	}
	ret = start_simulation(&data);
	end_simulation(&data);
	return (ret);
}
