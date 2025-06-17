/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrads <vrads@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:06:08 by vrads             #+#    #+#             */
/*   Updated: 2025/06/17 16:41:44 by vrads            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Releases the forks held by a philosopher.
 *
 * Unlocks the right fork first (if it exists and is held), then the left fork.
 * This order helps in preventing deadlocks, though primary deadlock prevention
 * is handled by the fork acquisition order in `take_forks`.
 *
 * @param philo Pointer to the t_philo structure representing the philosopher.
 */
void	drop_forks(t_philo *philo)
{
	if (philo->right_fork)
		pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

/**
 * @brief Acquires two forks for a philosopher to eat.
 *
 * Implements a strategy to prevent deadlocks by varying the order of
 * fork acquisition based on philosopher ID (even/odd).
 * Even ID philosophers pick left then right.
 * Odd ID philosophers pick right then left.
 * Prints a status message after acquiring each fork.
 *
 * @param philo Pointer to the t_philo structure representing the philosopher.
 */
void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork", 0);
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork", 0);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork", 0);
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork", 0);
	}
}

/**
 * @brief Simulates a philosopher eating.
 *
 * If the simulation is not over, this function:
 * 1. Prints an "is eating" status.
 * 2. Updates the philosopher's state to EATING.

	* 3. Locks the meal_time_mutex to safely update `last_meal_time` and `meals_eaten`.
 * 4. Simulates the eating duration using `precise_usleep`.
 * 5. Calls `drop_forks` to release the forks.
 * 6. Sets the philosopher's state to SLEEPING.
 *
 * @param philo Pointer to the t_philo structure representing the philosopher.
 */
void	eat(t_philo *philo)
{
	if (is_simulation_over(philo->table))
		return ;
	print_status(philo, "is eating", 0);
	philo->state = EATING;
	pthread_mutex_lock(&philo->table->meal_time_mutex);
	philo->last_meal_time = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->table->meal_time_mutex);
	precise_usleep(philo->table->time_to_eat, philo->table);
	drop_forks(philo);
	philo->state = SLEEPING;
}

/**
 * @brief Simulates a philosopher sleeping.
 *
 * If the simulation is not over, this function:
 * 1. Prints an "is sleeping" status.
 * 2. Simulates the sleeping duration using `precise_usleep`.

	* The philosopher's state is expected to be set to SLEEPING prior to calling this,
 * typically after eating.
 *
 * @param philo Pointer to the t_philo structure representing the philosopher.
 */
void	sleep_philo(t_philo *philo)
{
	if (is_simulation_over(philo->table))
		return ;
	print_status(philo, "is sleeping", 0);
	precise_usleep(philo->table->time_to_sleep, philo->table);
}

/**
 * @brief Simulates a philosopher thinking.
 *
 * If the simulation is not over, this function:
 * 1. Prints an "is thinking" status.
 * 2. Sets the philosopher's state to THINKING.
 * 3. Optionally, introduces a small delay to make thinking phase more explicit
 *    and to potentially improve fairness if `time_to_eat > time_to_sleep`.
 *    This delay is calculated to be short and not cause starvation.
 *
 * @param philo Pointer to the t_philo structure representing the philosopher.
 */
void	think(t_philo *philo)
{
	long long	think_time;
	long long	time_since_last_meal;

	if (is_simulation_over(philo->table))
		return ;
	print_status(philo, "is thinking", 0);
	philo->state = THINKING;
	if (philo->table->num_philos > 1
		&& philo->table->time_to_eat > philo->table->time_to_sleep)
	{
		think_time = (philo->table->time_to_eat - philo->table->time_to_sleep)
			/ 2;
		if (think_time <= 0)
			think_time = 1;
		pthread_mutex_lock(&philo->table->meal_time_mutex);
		time_since_last_meal = get_time_ms() - philo->last_meal_time;
		pthread_mutex_unlock(&philo->table->meal_time_mutex);
		if (time_since_last_meal + think_time < philo->table->time_to_die)
		{
			precise_usleep(think_time, philo->table);
		}
	}
}
