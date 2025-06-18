/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vflorez <vflorez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:05:49 by vrads             #+#    #+#             */
/*   Updated: 2025/06/18 13:24:15 by vflorez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Prints the command-line usage instructions for the program.
 *
 * This function outputs the expected arguments and their order to stdout.
 */
void	print_usage(void)
{
	printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat "
		"time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	printf("All time arguments should be in milliseconds.\n");
}

/**
 * @brief Main entry point for the Dining Philosophers simulation.
 *
 * Parses command-line arguments, initializes simulation state, launches
 * philosopher and monitor threads, waits for simulation completion,
 * and cleans up resources.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 *             Expected arguments:
 *             1. number_of_philosophers
 *             2. time_to_die (ms)
 *             3. time_to_eat (ms)
 *             4. time_to_sleep (ms)
 *             5. [number_of_times_each_philosopher_must_eat] (optional)
 * @return 0 on successful completion, 1 on error (e.g., invalid arguments,
 *         initialization failure, thread creation failure).
 */
int	main(int argc, char **argv)
{
	t_table		table;
	pthread_t	monitor_thread;

	if (argc < 5 || argc > 6)
	{
		print_usage();
		return (1);
	}
	if (initialize_simulation(&table, argc, argv) != 0)
	{
		return (1);
	}
	if (launch_threads(&table, &monitor_thread) != 0)
	{
		cleanup(&table);
		return (1);
	}
	pthread_join(monitor_thread, NULL);
	cleanup(&table);
	return (0);
}
