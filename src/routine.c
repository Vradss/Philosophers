#include "philo.h"

/**
 * @brief Handles the specific behavior for a single philosopher.
 *
 * A single philosopher will take their one fork, print a status message,
 * and then wait (effectively until they die, as they cannot eat).
 * This function is called when `table->num_philos == 1`.
 *
 * @param philo Pointer to the t_philo structure for the single philosopher.
 */
static void	handle_single_philosopher(t_philo *philo)
{
	print_status(philo, "has taken a fork", 0);
	precise_usleep(philo->table->time_to_die * 2, philo->table);
}

/**
 * @brief Performs one cycle of actions for a philosopher (take forks, eat, sleep, think).
 *
 * This function orchestrates the sequence of actions a philosopher performs:
 * 1. `take_forks()`: Acquires the necessary forks.
 * 2. `eat()`: Simulates eating.
 * 3. `sleep_philo()`: Simulates sleeping.
 * 4. `think()`: Simulates thinking.
 * After `take_forks` and each action, it checks if the simulation has ended.
 * If the simulation ends during `take_forks` (before eating), forks are dropped.
 *
 * @param philo Pointer to the t_philo structure for the philosopher.
 * @return 0 if the cycle completed and simulation can continue,
 *         1 if the simulation ended during the cycle.
 */
static int	perform_cycle_actions(t_philo *philo)
{
	take_forks(philo);
	if (is_simulation_over(philo->table))
	{
		drop_forks(philo);
		return (1);
	}
	eat(philo);
	if (is_simulation_over(philo->table))
		return (1);
	sleep_philo(philo);
	if (is_simulation_over(philo->table))
		return (1);
	think(philo);
	return (0);
}

/**
 * @brief The main routine executed by each philosopher thread.
 *
 * Initializes the philosopher structure from the argument.
 * Delays the start of even ID philosophers to prevent immediate deadlock.
 * Handles the special case of a single philosopher by calling `handle_single_philosopher`.
 * For multiple philosophers, it enters a loop, calling `perform_cycle_actions`
 * until the simulation ends.
 *
 * @param arg Pointer to the t_philo structure for this philosopher, passed as `void*`.
 * @return NULL when the philosopher's routine is complete or simulation ends.
 */
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		precise_usleep(philo->table->time_to_eat / 10, philo->table);

	if (philo->table->num_philos == 1)
	{
		handle_single_philosopher(philo);
		return (NULL);
	}

	while (!is_simulation_over(philo->table))
	{
		if (perform_cycle_actions(philo))
			break;
	}
	return (NULL);
}
