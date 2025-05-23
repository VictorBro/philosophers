/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:48:51 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/23 17:13:34 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <semaphore.h>
# include <string.h>
# include <stdbool.h>
# include <errno.h>

# define FALSE 0
# define TRUE 1

/* Semaphore names */
# define SEM_FORKS "/philo_forks"
# define SEM_WRITE "/philo_write"
# define SEM_DEAD "/philo_dead"
# define SEM_MEALS "/philo_meals"
# define SEM_EATERS "/philo_eaters"
# define SEM_MEAL_TIME "/philo_meal_time_"

/* Error codes */
enum
{
	OK,
	ARG_ERROR,
	ALLOC_ERROR,
	SEMAPHORE_ERROR,
	FORK_ERROR
};

/* Main data structure */
typedef struct s_data
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_times_to_eat;
	pid_t			*pids;
	sem_t			*forks;
	sem_t			*write_sem;
	sem_t			*dead_sem;
	sem_t			*meal_sem;
	sem_t			*eaters_sem;
}	t_data;

/* Philosopher structure */
typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		monitor_thread;
	sem_t			*meal_time_sem;
	char			meal_time_sem_name[30];
	t_data			*data;
}	t_philo;

/* Utility functions declarations */
int		ft_error(const char *msg, int ret_val);
void	print_status(t_philo *philo, char *status);
int		get_time_ms(long *current_time);
void	precise_sleep(long milliseconds);
int		parse_int(const char *str);

/* Initialization functions */
int		init_data(t_data *data, int argc, char **argv);
void	parse_arguments(t_data *data, int argc, char **argv);
int		validate_arguments(t_data *data, int argc);
void	cleanup_semaphores(void);

/* Simulation functions */
int		start_simulation(t_data *data);
int		create_philosophers(t_data *data);
void	end_simulation(t_data *data);
void	kill_philosophers(t_data *data);
void	wait_for_philosophers(t_data *data);
int		wait_for_meal_completion(t_data *data);

/* Monitoring functions */
void	clean_child_exit(t_philo *philo, int status);
void	*monitor_routine(void *arg);
void	philo_check_death(t_philo *philo, long current_time, long time_since);

/* Philosopher functions */
void	philosopher_routine(t_data *data, int philo_id);
void	init_philo(t_philo *philo, t_data *data, int philo_id);
void	stagger_start(t_philo *philo);
void	handle_single_philo(t_philo *philo);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	check_meals_and_exit(t_philo *philo);
void	check_death_during_sleep(t_philo *philo, long current_time);
void	safe_sleep(t_philo *philo, long duration);
void	handle_single_philo_eat(t_philo *philo);
void	calculate_sleep_time(long time_remaining, long *sleep_time);

#endif
