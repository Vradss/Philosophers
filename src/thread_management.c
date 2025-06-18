/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vflorez <vflorez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:05:54 by vrads             #+#    #+#             */
/*   Updated: 2025/06/18 13:22:06 by vflorez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Initializes all components of the simulation.
 *
 * Calls `init_table` to parse arguments and set up basic table data,
 * then `init_mutexes` to prepare all necessary mutexes, and finally
 * `init_philos` to set up the philosopher structures.
 *
 * @param table Pointer to the t_table structure to be initialized.
 * @param argc Argument count from main.
 * @param argv Argument vector from main.
 * @return 0 if all initialization steps are successful, 1 on any error.
 */
int	initialize_simulation(t_table *table, int argc, char **argv)
{
	if (init_table(table, argc, argv) != 0)
		return (1);
	if (init_mutexes(table) != 0)
		return (1);
	if (init_philos(table) != 0)
		return (1);
	return (0);
}

/**
 * @brief Handles errors during philosopher thread creation.
 *
 * Prints an error message, sets the simulation end flag, and attempts to join
 * all previously created and validated philosopher threads.
 *
 * @param table Pointer to the t_table structure.

	* @param failed_philo_idx Index of the philosopher whose thread 
	creation failed.

	* @param num_created_threads Number of philosopher threads 
	successfully created before the failure.
 */
static void	handle_thread_creation_error(t_table *table, int failed_philo_idx,
		int num_created_threads)
{
	int	j;

	printf("Error: pthread_create failed for philo %d\n", failed_philo_idx + 1);
	pthread_mutex_lock(&table->sim_end_mutex);
	table->simulation_should_end = 1;
	pthread_mutex_unlock(&table->sim_end_mutex);
	j = 0;
	while (j < num_created_threads)
	{
		if (table->philos[j].thread_valid)
			pthread_join(table->philos[j].thread, NULL);
		j++;
	}
}

/**
 * @brief Creates and launches all philosopher threads.
 *
 * Iterates from 0 to `table->num_philos - 1`. In each iteration:
 * 1. Sets the philosopher's `last_meal_time` to the simulation `start_time`.

	* 2. Attempts to create a new thread for the philosopher using 
	`pthread_create`,
 *    with `philosopher_routine` as the thread function.
 * 3. If thread creation fails, calls `handle_thread_creation_error` to manage
 *    the error and cleanup, then returns 1.
 * 4. Marks the philosopher's thread as valid upon successful creation.
 *
 * @param table Pointer to the t_table structure containing philosopher 
	data and settings.
 * @param start_time The official start time of the simulation 
	(in milliseconds).
 * @return 0 if all philosopher threads are created successfully, 1 on error.
 */
static int	create_philosopher_threads(t_table *table, long long start_time)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		table->philos[i].last_meal_time = start_time;
		if (pthread_create(&table->philos[i].thread, NULL, philosopher_routine,
				&table->philos[i]) != 0)
		{
			handle_thread_creation_error(table, i, i);
			return (1);
		}
		table->philos[i].thread_valid = 1;
		i++;
	}
	return (0);
}

/**
 * @brief Creates and launches the monitoring thread.
 *

 * Attempts to create a new thread for the monitoring routine. 
 * If creation fails, it prints an error message and sets 
 * the simulation end flag.
 *
 * @param table Pointer to the t_table structure.
 * @param monitor_thread_id Pointer to a pthread_t variable where the ID of the
 *                          created monitor thread will be stored.
 * @return 0 if the monitor thread is created successfully, 1 on error.
 */
int	create_monitor_thread(t_table *table, pthread_t *monitor_thread_id)
{
	if (pthread_create(monitor_thread_id, NULL, monitoring_routine, table) != 0)
	{
		printf("Error: pthread_create failed for monitor thread\n");
		pthread_mutex_lock(&table->sim_end_mutex);
		table->simulation_should_end = 1;
		pthread_mutex_unlock(&table->sim_end_mutex);
		return (1);
	}
	return (0);
}

/**
 * @brief Launches all threads for the simulation (philosophers and monitor).
 *

	* 1. Records the simulation start time using `get_time_ms()` and stores 
	it in `table->start_time`.

	* 2. Calls `create_philosopher_threads` to create and start all 
	philosopher threads.
 *    If this fails, returns 1.
 * 3. Calls `create_monitor_thread` to create and start the monitoring thread.
 *    If this fails, returns 1.
 *
 * @param table Pointer to the t_table structure.

	* @param monitor_thread_id Pointer to a pthread_t variable for storing 
	the monitor thread ID.
 * @return 0 if all threads are launched successfully, 1 on any error.
 */
int	launch_threads(t_table *table, pthread_t *monitor_thread_id)
{
	long long	start_time;

	start_time = get_time_ms();
	table->start_time = start_time;
	if (create_philosopher_threads(table, start_time) != 0)
	{
		return (1);
	}
	if (create_monitor_thread(table, monitor_thread_id) != 0)
	{
		return (1);
	}
	return (0);
}
