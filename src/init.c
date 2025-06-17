/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrads <vrads@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:06:52 by vrads             #+#    #+#             */
/*   Updated: 2025/06/17 16:06:53 by vrads            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Parses the command-line arguments to initialize table settings.
 *
 * Populates the `t_table` structure with values from `argv`, including
 * number of philosophers, time to die, time to eat, time to sleep, and
 * optionally, the number of times each philosopher must eat.
 * Validates the arguments to ensure they are positive integers and that
 * the number of philosophers does not exceed a predefined limit (e.g., 200).
 * Prints usage instructions if arguments are invalid.
 *
 * @param table Pointer to the t_table structure to be initialized.
 * @param argc The argument count.
 * @param argv The argument vector.
 * @return 0 if arguments are parsed successfully, 1 on error (invalid arguments).
 */
static int	parse_args(t_table *table, int argc, char **argv)
{
	table->num_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->num_must_eat = ft_atoi(argv[5]);
	else
		table->num_must_eat = -1;
	if (table->num_philos <= 0 || table->time_to_die <= 0 || \
		table->time_to_eat <= 0 || table->time_to_sleep <= 0 || \
		(argc == 6 && table->num_must_eat <= 0))
	{
		printf("Error: Invalid arguments.\n");
		print_usage();
		return (1);
	}
	if (table->num_philos > 200)
	{
		printf("Error: Number of philosophers cannot exceed 200.\n");
		return (1);
	}
	return (0);
}

/**
 * @brief Initializes the main simulation table structure.
 *
 * This function first parses the command-line arguments using `parse_args`.
 * If parsing is successful, it initializes other members of the `t_table`
 * structure to their default values (e.g., start_time to 0, simulation_should_end
 * to 0, pointers to NULL, and mutex initialized flags to 0).
 *
 * @param table Pointer to the t_table structure to be initialized.
 * @param argc The argument count from main.
 * @param argv The argument vector from main.
 * @return 0 if initialization is successful, 1 if argument parsing fails.
 */
int	init_table(t_table *table, int argc, char **argv)
{
	if (parse_args(table, argc, argv) != 0)
		return (1);
	table->start_time = 0;
	table->simulation_should_end = 0;
	table->philos = NULL;
	table->forks = NULL;
	table->forks_initialized_count = 0;
	table->print_mutex_initialized = 0;
	table->sim_end_mutex_initialized = 0;
	table->meal_time_mutex_initialized = 0;
	return (0);
}
