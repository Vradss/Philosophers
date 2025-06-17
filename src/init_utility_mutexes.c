/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utility_mutexes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrads <vrads@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:07:40 by vrads             #+#    #+#             */
/*   Updated: 2025/06/17 16:45:02 by vrads            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Initializes the print mutex.
 *
 * This mutex is used to synchronize printing status messages to the console,
 * ensuring that messages from different threads do not interleave incorrectly.
 *
 * @param table Pointer to the t_table structure where the mutex is stored.
 * @return 0 if successful, 1 on error from pthread_mutex_init.
 */
static int	init_print_mutex_internal(t_table *table)
{
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
	{
		printf("Error: Mutex init failed for print_mutex.\n");
		return (1);
	}
	table->print_mutex_initialized = 1;
	return (0);
}

/**
 * @brief Initializes the simulation end mutex.
 *
 * This mutex protects the `simulation_should_end` flag in the t_table structure,
 * ensuring that checks and updates to this flag are atomic.
 *
 * @param table Pointer to the t_table structure where the mutex is stored.
 * @return 0 if successful, 1 on error from pthread_mutex_init.
 */
static int	init_sim_end_mutex_internal(t_table *table)
{
	if (pthread_mutex_init(&table->sim_end_mutex, NULL) != 0)
	{
		printf("Error: Mutex init failed for sim_end_mutex.\n");
		return (1);
	}
	table->sim_end_mutex_initialized = 1;
	return (0);
}

/**
 * @brief Initializes the meal time mutex.
 *
 * This mutex protects access to `last_meal_time` and `meals_eaten` for each
 * philosopher,
	ensuring thread-safe updates and reads of these critical variables.
 *
 * @param table Pointer to the t_table structure where the mutex is stored.
 * @return 0 if successful, 1 on error from pthread_mutex_init.
 */
static int	init_meal_time_mutex_internal(t_table *table)
{
	if (pthread_mutex_init(&table->meal_time_mutex, NULL) != 0)
	{
		printf("Error: Mutex init failed for meal_time_mutex.\n");
		return (1);
	}
	table->meal_time_mutex_initialized = 1;
	return (0);
}

/**
 * @brief Initializes all utility mutexes (print, simulation end, meal time).
 *
 * Sequentially initializes the print mutex, then the simulation end mutex,
 * and finally the meal time mutex. If any initialization fails, it attempts
 * to destroy any previously successfully initialized utility mutexes in this function
 * before returning an error.
 *
 * @param table Pointer to the t_table structure to store the mutexes.
 * @return 0 if all utility mutexes are initialized successfully, 1 on error.
 */
int	init_utility_mutexes(t_table *table)
{
	if (init_print_mutex_internal(table) != 0)
		return (1);
	if (init_sim_end_mutex_internal(table) != 0)
	{
		if (table->print_mutex_initialized)
			pthread_mutex_destroy(&table->print_mutex);
		table->print_mutex_initialized = 0;
		return (1);
	}
	if (init_meal_time_mutex_internal(table) != 0)
	{
		if (table->print_mutex_initialized)
			pthread_mutex_destroy(&table->print_mutex);
		table->print_mutex_initialized = 0;
		if (table->sim_end_mutex_initialized)
			pthread_mutex_destroy(&table->sim_end_mutex);
		table->sim_end_mutex_initialized = 0;
		return (1);
	}
	return (0);
}
