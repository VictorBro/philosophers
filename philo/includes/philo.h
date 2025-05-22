/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:46:20 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/22 19:26:19 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>
# include <string.h>
# include <errno.h>

# define FALSE	0
# define TRUE	1

enum
{
	OK,
	ARG_ERROR,
	ALLOC_ERROR,
	INTERNAL_ERROR,
	DIED_ERROR,
};

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	dead_mutex;
	bool			someone_died;
	int				all_ate;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
}	t_philo;

/* time_utils.c */
int		get_time(long *current_time);
void	precise_sleep(long ms);

/* print_utils.c */
void	print_state(t_data *data, int id, char *state);

/* check_utils.c */
bool	should_stop(t_data *data);

/* fork_utils.c */
bool	take_forks(t_philo *philo);
void	release_forks(t_philo *philo);

/* eat_utils.c */
void	eat(t_philo *philo);

/* thread_routines.c */
void	*philosopher(void *arg);

/* init_utils.c */
bool	init_mutexes(t_data *data, pthread_mutex_t **forks);

/* philo_init_utils.c */
bool	init_philosophers(t_data *data, t_philo **philos,
			pthread_mutex_t *forks);

/* simulation_utils.c */
bool	start_simulation(t_data *data, t_philo *philos);
void	mark_death(t_data *data);

/* monitor_utils.c */
bool	monitor_philosophers(t_data *data, t_philo *philos);

/* cleanup_utils.c */
void	cleanup(t_data *data, t_philo *philos, pthread_mutex_t *forks);

/* parse_utils.c */
bool	parse_args(int argc, char **argv, t_data *data);
int		ft_error(const char *msg, int ret_val);

#endif
