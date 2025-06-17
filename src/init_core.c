#include "philo.h"

/**
 * @brief Initializes all mutexes required for the simulation.
 *
 * This function initializes fork mutexes by calling `init_fork_mutexes`
 * and then initializes utility mutexes (print, simulation end, meal time)
 * by calling `init_utility_mutexes`. If `init_utility_mutexes` fails,
 * it cleans up the already initialized fork mutexes before returning an error.
 *
 * @param table Pointer to the t_table structure where mutexes are stored
 *              and their initialization status is tracked.
 * @return 0 if all mutexes are initialized successfully, 1 on error.
 */
int	init_mutexes(t_table *table)
{
	if (init_fork_mutexes(table) != 0)
	{
		return (1);
	}
	if (init_utility_mutexes(table) != 0)
	{
		destroy_n_fork_mutexes(table, table->forks_initialized_count);
		return (1);
	}
	return (0);
}

/**
 * @brief Initializes the philosopher structures.
 *
 * Allocates memory for the array of philosopher structures (`t_philo`) within
 * the `t_table` structure. Initializes each philosopher with their ID,
 * default meal count, initial state (THINKING), a pointer to the table,
 * and pointers to their left and right forks.
 * Special handling for a single philosopher: their right_fork is set to NULL.
 *
 * @param table Pointer to the t_table structure which contains the philosophers
 *              array and simulation parameters like `num_philos` and `forks`.
 * @return 0 if philosophers are initialized successfully, 1 on malloc failure.
 */
int	init_philos(t_table *table)
{
	int	i;

	table->philos = malloc(sizeof(t_philo) * table->num_philos);
	if (!table->philos)
	{
		printf("Error: Malloc failed for philosophers.\n");
		return (1);
	}
	i = 0;
	while (i < table->num_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].last_meal_time = 0;
		table->philos[i].thread_valid = 0;
		table->philos[i].state = THINKING;
		table->philos[i].table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->num_philos];
		if (table->num_philos == 1)
			table->philos[i].right_fork = NULL;
		i++;
	}
	return (0);
}
