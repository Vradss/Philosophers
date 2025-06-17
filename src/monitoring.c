/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrads <vrads@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:06:59 by vrads             #+#    #+#             */
/*   Updated: 2025/06/17 16:07:00 by vrads            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Checks if a philosopher has died due to starvation.
 *
 * Calculates the time since the philosopher's last meal. If this time
 * exceeds `time_to_die`, the philosopher is marked as dead, the simulation
 * end flag is set (if not already set), and a "died" status is printed.
 *
 * @param philo Pointer to the t_philo structure for the philosopher to check.
 * @return 1 if the philosopher has died or the simulation has already ended,
 *         0 otherwise.
 */
int	check_death(t_philo *philo)
{
	long long	time_since_last_meal;

	pthread_mutex_lock(&philo->table->meal_time_mutex);
	time_since_last_meal = get_time_ms() - philo->last_meal_time;
	pthread_mutex_unlock(&philo->table->meal_time_mutex);

	if (time_since_last_meal > philo->table->time_to_die)
	{
		pthread_mutex_lock(&philo->table->sim_end_mutex);
		if (!philo->table->simulation_should_end)
		{
			philo->table->simulation_should_end = 1;
			pthread_mutex_unlock(&philo->table->sim_end_mutex);
			print_status(philo, "died", 1);
			philo->state = DEAD;
			return (1);
		}
		pthread_mutex_unlock(&philo->table->sim_end_mutex);
		return (1);
	}
	return (0);
}

/**
 * @brief Checks if a single philosopher is not yet full and simulation is running.
 *
 * This is a helper for `check_all_full`. It checks if the given philosopher
 * has eaten fewer meals than `num_must_eat`. If the philosopher is full and
 * the simulation hasn't ended, their state is updated to FULL.
 *
 * @param philo Pointer to the t_philo structure to check.
 * @return 1 if the philosopher is not full and simulation is running, 0 otherwise
 *         (philosopher is full or simulation has ended).
 */
static int	is_philo_not_full_and_sim_running(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->meal_time_mutex);
	if (philo->meals_eaten < philo->table->num_must_eat)
	{
		pthread_mutex_unlock(&philo->table->meal_time_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->meal_time_mutex);

	pthread_mutex_lock(&philo->table->sim_end_mutex);
	if (!philo->table->simulation_should_end)
	{
		philo->state = FULL;
	}
	pthread_mutex_unlock(&philo->table->sim_end_mutex);
	return (0);
}

/**
 * @brief Finalizes simulation if all philosophers are full.
 *
 * This helper for `check_all_full` sets the simulation end flag if
 * `all_philos_are_full_flag` is true and the simulation hasn't already ended.
 *
 * @param table Pointer to the t_table structure.
 * @param all_philos_are_full_flag Integer flag (1 if all philosophers are full, 0 otherwise).
 * @return 1 if the simulation was ended by this call, 0 otherwise.
 */
static int	finalize_if_all_full(t_table *table, int all_philos_are_full_flag)
{
	if (all_philos_are_full_flag)
	{
		pthread_mutex_lock(&table->sim_end_mutex);
		if (!table->simulation_should_end)
		{
			table->simulation_should_end = 1;
		}
		pthread_mutex_unlock(&table->sim_end_mutex);
		return (1);
	}
	return (0);
}

/**
 * @brief Checks if all philosophers have eaten the required number of meals.
 *
 * This function is active only if `num_must_eat` was specified. It iterates
 * through all philosophers. If any philosopher has not yet eaten `num_must_eat`
 * meals, the function returns 0. If all philosophers are full, it calls
 * `finalize_if_all_full` to potentially end the simulation.
 *
 * @param table Pointer to the t_table structure.
 * @return 1 if all philosophers are full and simulation is ended by this check,
 *         0 otherwise (or if `num_must_eat` is not set).
 */
int	check_all_full(t_table *table)
{
	int	i;
	int	all_philos_considered_full;

	if (table->num_must_eat == -1)
		return (0);
	all_philos_considered_full = 1;
	i = 0;
	while (i < table->num_philos)
	{
		if (is_philo_not_full_and_sim_running(&table->philos[i]))
		{
			all_philos_considered_full = 0;
			break;
		}
		i++;
	}
	return (finalize_if_all_full(table, all_philos_considered_full));
}

/**
 * @brief The main routine for the monitoring thread.
 *
 * Continuously checks the status of all philosophers. In each iteration:
 * 1. It checks each philosopher for death using `check_death`.
 * 2. It checks if all philosophers are full using `check_all_full` (if applicable).
 * If either condition causes the simulation to end, the monitoring routine exits.
 * A small delay (`usleep`) is introduced to prevent busy-waiting and reduce CPU usage.
 * It also checks `is_simulation_over` periodically to exit if another thread
 * (like a failed philosopher thread creation) has ended the simulation.
 *
 * @param arg Pointer to the t_table structure, passed as `void*`.
 * @return NULL when the simulation ends.
 */
void	*monitoring_routine(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (1)
	{
		i = 0;
		while (i < table->num_philos)
		{
			if (check_death(&table->philos[i]) || check_all_full(table))
			{
				return (NULL);
			}
			i++;
		}
		usleep(1000);
		if (is_simulation_over(table))
			return (NULL);
	}
	return (NULL);
}
