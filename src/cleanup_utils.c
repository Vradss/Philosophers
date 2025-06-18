/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrads <vrads@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:07:44 by vrads             #+#    #+#             */
/*   Updated: 2025/06/17 18:02:50 by vrads            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Joins all valid philosopher threads.
 *
 * Iterates through all philosophers in the table. If a philosopher's
 * thread is marked as valid (i.e., successfully created), this function
 * waits for that thread to terminate using `pthread_join`.
 *
 * @param table Pointer to the t_table structure containing philosopher data.
 */
static void	join_philosopher_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		if (table->philos[i].thread_valid)
			pthread_join(table->philos[i].thread, NULL);
		i++;
	}
}

/**
 * @brief Destroys all initialized fork mutexes.
 *
 * Iterates up to `table->forks_initialized_count`, destroying each fork mutex.
 * After destroying the mutexes,
	it frees the memory allocated for the forks array
 * and resets `table->forks` to NULL and `table->forks_initialized_count` to 0.
 * This function was originally in main.c and is specific to cleanup.
 *
 * @param table Pointer to the t_table structure.
 */
static void	destroy_main_fork_mutexes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->forks_initialized_count)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
	table->forks = NULL;
	table->forks_initialized_count = 0;
}

/**
 * @brief Destroys all initialized utility mutexes (print, simulation end,
	meal time).
 *
 * Checks if each utility mutex was initialized and, if so, destroys it
 * and resets its initialization flag.
 *
 * @param table Pointer to the t_table structure.
 */
static void	destroy_all_utility_mutexes(t_table *table)
{
	if (table->print_mutex_initialized)
	{
		pthread_mutex_destroy(&table->print_mutex);
		table->print_mutex_initialized = 0;
	}
	if (table->sim_end_mutex_initialized)
	{
		pthread_mutex_destroy(&table->sim_end_mutex);
		table->sim_end_mutex_initialized = 0;
	}
	if (table->meal_time_mutex_initialized)
	{
		pthread_mutex_destroy(&table->meal_time_mutex);
		table->meal_time_mutex_initialized = 0;
	}
}

/**
 * @brief Cleans up all resources used by the simulation.
 *
 * This function performs the following cleanup steps:
 * 1. Joins all philosopher threads if philosophers array is allocated.
 * 2. Frees the philosophers array.
 * 3. Destroys and frees fork mutexes if they were initialized.
 * 4. Destroys utility mutexes (print, simulation end,
	meal time) if initialized.
 *
 * @param table Pointer to the t_table structure containing all simulation data.
 *              If NULL, the function returns immediately.
 */
void	cleanup(t_table *table)
{
	if (!table)
		return ;
	if (table->philos)
	{
		join_philosopher_threads(table);
		free(table->philos);
		table->philos = NULL;
	}
	if (table->forks && table->forks_initialized_count > 0)
	{
		destroy_main_fork_mutexes(table);
	}
	destroy_all_utility_mutexes(table);
}
