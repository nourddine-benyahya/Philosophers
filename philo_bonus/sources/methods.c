/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:30:47 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/23 21:20:32 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	meal_method(t_philo *philo, char method, long long new_val)
{
	long long	meal;

	meal = new_val;
	sem_wait(philo->meal);
	if (method == 'g')
		meal = philo->last_meal;
	else if (method == 's')
		philo->last_meal = new_val;
	sem_post(philo->meal);
	return (meal);
}
void	eat(t_philo *philo, long long *i)
{
	sem_wait(philo->env->forks);
	actions(philo, RIGHT_FORK, 0);
	sem_wait(philo->env->forks);
	actions(philo, LEFT_FORK, 1);
	// meal_method(philo, 's', time_stamp());
	n3ass(philo->env->time_to_eat);
	*i += 1;
	if (philo->env->nbr_must_eat != -1 && *i == philo->env->nbr_must_eat)
		sem_post(philo->env->meal_nbr);
	sem_post(philo->env->forks);
	sem_post(philo->env->forks);
}
