/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:00:03 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/22 12:06:08 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	status_method(t_philo *philos, char method, bool new_val)
{
	bool	status;

	status = new_val;
	pthread_mutex_lock(&philos->env->status_mutex);
	if (method == 'g')
		status = philos->env->status;
	else if (method == 's')
		philos->env->status = new_val;
	pthread_mutex_unlock(&philos->env->status_mutex);
	return (status);
}

long long	meal_method(t_philo *philos, char method, long long new_val)
{
	long long	meal;

	meal = new_val;
	pthread_mutex_lock(&philos->meal);
	if (method == 'g')
		meal = philos->last_meal;
	else if (method == 's')
		philos->last_meal = new_val;
	pthread_mutex_unlock(&philos->meal);
	return (meal);
}

int	meal_nbr_total(t_philo *philos, char method, int new_val)
{
	int	meal;

	meal = new_val;
	pthread_mutex_lock(&philos->env->nbr_must_eat_total_mutex);
	if (method == 'g')
		meal = philos->env->nbr_must_eat_total;
	else if (method == 's')
		philos->env->nbr_must_eat_total = new_val;
	pthread_mutex_unlock(&philos->env->nbr_must_eat_total_mutex);
	return (meal);
}
