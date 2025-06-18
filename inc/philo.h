/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrads <vrads@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:05:43 by vrads             #+#    #+#             */
/*   Updated: 2025/06/17 17:52:53 by vrads            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

// Enum for philosopher states
typedef enum e_state
{
	EATING,
	SLEEPING,
	THINKING,
	DEAD,
	FULL
}					t_state;

// Structure for philosopher data
typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	int				thread_valid;
	pthread_t		thread;
	t_state			state;
	struct s_table	*table;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}					t_philo;

// Structure for table data (shared resources)
typedef struct s_table
{
	int				num_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				num_must_eat;
	long long		start_time;
	int				simulation_should_end;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	int				forks_initialized_count;
	pthread_mutex_t	print_mutex;
	int				print_mutex_initialized;
	pthread_mutex_t	sim_end_mutex;
	int				sim_end_mutex_initialized;
	pthread_mutex_t	meal_time_mutex;
	int				meal_time_mutex_initialized;
}					t_table;

// Function prototypes

// main.c
void				print_usage(void);

// cleanup_utils.c
void				cleanup(t_table *table);

// thread_management.c
int					initialize_simulation(t_table *table, int argc,
						char **argv);
int					launch_threads(t_table *table,
						pthread_t *monitor_thread_id);
int					create_monitor_thread(t_table *table,
						pthread_t *monitor_thread_id);

// utils.c
long long			get_time_ms(void);
int					ft_atoi(const char *str);
void				print_status(t_philo *philo, const char *status,
						int override_sim_end);
int					is_simulation_over(t_table *table);
void				precise_usleep(long long time_ms, t_table *table);

// init.c
int					init_table(t_table *table, int argc, char **argv);

// init_core.c
int					init_philos(t_table *table);
int					init_mutexes(t_table *table);

// init_forks.c
int					init_fork_mutexes(t_table *table);
void				destroy_n_fork_mutexes(t_table *table, int n);

// init_utility_mutexes.c
int					init_utility_mutexes(t_table *table);

// routine.c
void				*philosopher_routine(void *arg);
// actions.c
void				take_forks(t_philo *philo);
void				drop_forks(t_philo *philo);
void				eat(t_philo *philo);
void				sleep_philo(t_philo *philo);
void				think(t_philo *philo);

// monitoring.c
void				*monitoring_routine(void *arg);
int					check_death(t_philo *philo);
int					check_all_full(t_table *table);

#endif
