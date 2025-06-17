/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrads <vrads@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:07:36 by vrads             #+#    #+#             */
/*   Updated: 2025/06/17 16:07:37 by vrads            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Destroys a specified number of fork mutexes and frees related memory.
 *
 * Iterates `n` times, destroying each fork mutex from index 0 to `n-1`.
 * After destroying the mutexes, it frees the memory allocated for the `table->forks`
 * array, sets `table->forks` to NULL, and `table->forks_initialized_count` to 0.
 * This function is typically called during cleanup or when `init_fork_mutexes`
 * fails partway through initialization.
 *
 * @param table Pointer to the t_table structure containing the forks.
 * @param n The number of fork mutexes to destroy.
 */
void	destroy_n_fork_mutexes(t_table *table, int n)
{
	int	k;

	k = 0;
	while (k < n)
	{
		pthread_mutex_destroy(&table->forks[k]);
		k++;
	}
	free(table->forks);
	table->forks = NULL;
	table->forks_initialized_count = 0;
}

/**
 * @brief Initializes all fork mutexes for the simulation.
 *
 * Allocates memory for an array of `pthread_mutex_t` (one for each philosopher).
 * Then, iterates through the array, initializing each mutex. If any mutex
 * initialization fails, it prints an error, destroys all previously initialized
 * fork mutexes using `destroy_n_fork_mutexes`, and returns an error code.
 * If successful, sets `table->forks_initialized_count` to `table->num_philos`.
 *
 * @param table Pointer to the t_table structure which will store the initialized
 *              forks and tracking variables.
 * @return 0 if all fork mutexes are initialized successfully, 1 on error
 *         (malloc failure or mutex init failure).
 */
int	init_fork_mutexes(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philos);
	if (!table->forks)
	{
		printf("Error: Malloc failed for forks.\n");
		return (1);
	}
	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
		{
			printf("Error: Mutex init failed for fork %d.\n", i);
			destroy_n_fork_mutexes(table, i);
			return (1);
		}
		i++;
	}
	table->forks_initialized_count = table->num_philos;
	return (0);
}
