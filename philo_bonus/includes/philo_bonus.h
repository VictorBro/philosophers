/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:48:51 by vbronov           #+#    #+#             */
/*   Updated: 2025/05/01 18:01:53 by vbronov          ###   ########.fr       */
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
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>
# include <string.h>
# include <stdbool.h>

# define SEM_FORKS "/sem_forks"
# define SEM_WRITE "/sem_write"
# define SEM_DEAD "/sem_dead"
# define SEM_MEALS "/sem_meals"
# define SEM_DEADLOCK "/sem_deadlock"

typedef struct s_data
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_times_to_eat;
	long			start_time;
	sem_t			*forks;
	sem_t			*write_sem;
	sem_t			*dead_sem;
	sem_t			*meal_sem;
	sem_t			*deadlock_sem;
	pid_t			*pids;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	t_data			*data;
	pthread_t		monitor_thread;
}	t_philo;

/* Main functions */
int		init_data(t_data *data, int argc, char **argv);
int		start_simulation(t_data *data);
void	end_simulation(t_data *data);
void	clean_child_exit(t_philo *philo, int status);

/* Philosopher process */
void	philosopher_routine(t_data *data, int philo_id);
void	*monitor_routine(void *arg);

/* Actions */
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);

/* Utils */
int		ft_atoi(const char *str);
long	get_time(void);
void	print_status(t_philo *philo, char *status);
void	precise_sleep(long milliseconds);
void	cleanup_semaphores(void);

#endif
