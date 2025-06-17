/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrads <vrads@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:07:10 by vrads             #+#    #+#             */
/*   Updated: 2025/06/17 17:35:41 by vrads            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Gets the current time in milliseconds.
 *
 * Uses `gettimeofday` to retrieve the current time and converts it
 * to milliseconds.
 *
 * @return The current time in milliseconds since the Epoch.
 */
long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief Converts a string to an integer.
 *
 * Parses a string representation of an integer and returns its `int` value.
 * Handles leading whitespace and optional sign (+ or -). Stops parsing
 * at the first non-digit character.
 *
 * @param str The string to convert.
 * @return The integer value represented by the string.
 */
int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

/**
 * @brief Prints a philosopher's status message.
 *
 * Prints a timestamped status message for a philosopher, such as
 * "is eating", "is sleeping", "is thinking", "has taken a fork", or "died".
 * Message printing is synchronized using `table->print_mutex`.
 * The function first checks if the simulation should end (unless overridden)
 * before attempting to print.
 *
 * @param philo Pointer to the t_philo structure of the philosopher.
 * @param status The status message string to print.
 * @param override_sim_end If non-zero,
	print status even if simulation_should_end is set
 *                         (e.g., for death messages).
 */
void	print_status(t_philo *philo, const char *status, int override_sim_end)
{
	long long	time_ms;

	pthread_mutex_lock(&philo->table->sim_end_mutex);
	if (philo->table->simulation_should_end && !override_sim_end)
	{
		pthread_mutex_unlock(&philo->table->sim_end_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->sim_end_mutex);
	time_ms = get_time_ms() - philo->table->start_time;
	pthread_mutex_lock(&philo->table->print_mutex);
	if (!is_simulation_over(philo->table) || override_sim_end)
	{
		printf("%lld %d %s\n", time_ms, philo->id, status);
	}
	pthread_mutex_unlock(&philo->table->print_mutex);
}

/**
 * @brief Checks if the simulation is over.
 *
 * Safely reads the `simulation_should_end` flag from the table structure
 * using the `sim_end_mutex`.
 *
 * @param table Pointer to the t_table structure.
 * @return Non-zero if the simulation should end, 0 otherwise.
 */
int	is_simulation_over(t_table *table)
{
	int	ret;

	pthread_mutex_lock(&table->sim_end_mutex);
	ret = table->simulation_should_end;
	pthread_mutex_unlock(&table->sim_end_mutex);
	return (ret);
}

/**
 * @brief Simple and precise sleep implementation.
 *
 * @param time_ms The time to sleep in milliseconds.
 * @param table Pointer to the t_table structure for simulation checks.
 */
void	precise_usleep(long long time_ms, t_table *table)
{
	long long	start;
	long long	elapsed;

	start = get_time_ms();
	while (!is_simulation_over(table))
	{
		elapsed = get_time_ms() - start;
		if (elapsed >= time_ms)
			break ;
		usleep(500);
	}
}
