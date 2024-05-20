/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:44:51 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/15 13:04:11 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	n3ass(long long time, t_philo *philos)
{
	bool		status;
	long long	timer;

	status = true;
	timer = time_stamp();
	while (status)
	{
		if (time_stamp() - timer > time)
			break ;
		pthread_mutex_lock(&philos->env->status_mutex);
		status = philos->env->status;
		pthread_mutex_unlock(&philos->env->status_mutex);
		usleep(100);
	}
	return (0);
}

int	actions(t_philo *philos, char *action)
{
	int	status;

	pthread_mutex_lock(&philos->env->status_mutex);
	if (philos->env->status)
	{
		pthread_mutex_lock(&philos->env->printing);
		if (action == EATING && philos->eating_nbr != philos->env->nbr_must_eat)
		{
			philos->eating_nbr++;
			philos->env->nbr_must_eat_total--;
		}
		printf("%lld %d %s\n", time_stamp() - philos->env->time, philos->index,
			action);
		if (philos->env->nbr_must_eat_total == 0)
			philos->env->status = false;
		pthread_mutex_unlock(&philos->env->printing);
		status = 0;
	}
	else
		status = 1;
	pthread_mutex_unlock(&philos->env->status_mutex);
	return (status);
}

int	put_forks_down(pthread_mutex_t *right, pthread_mutex_t *left)
{
	if (right != NULL)
		pthread_mutex_unlock(left);
	if (right != NULL)
		pthread_mutex_unlock(right);
	return (0);
}

int	pickup_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->fork.right);
	if (actions(philo, RIGHT_FORK) == 1)
	{
		pthread_mutex_unlock(philo->fork.right);
		return (1);
	}
	if (philo->env->philo_num == 1)
	{
		n3ass(philo->env->time_to_die, philo);
		pthread_mutex_unlock(philo->fork.right);
		return (1);
	}
	pthread_mutex_lock(philo->fork.left);
	if (actions(philo, LEFT_FORK) == 1)
	{
		put_forks_down(philo->fork.right, philo->fork.left);
		return (1);
	}
	return (0);
}

int	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal);
	philo->last_meal = time_stamp();
	pthread_mutex_unlock(&philo->meal);
	if (actions(philo, EATING) == 1)
	{
		put_forks_down(philo->fork.right, philo->fork.left);
		return (1);
	}
	pthread_mutex_lock(&philo->meal);
	philo->last_meal = time_stamp();
	pthread_mutex_unlock(&philo->meal);
	if (n3ass(philo->env->time_to_eat, philo) == 1)
	{
		put_forks_down(philo->fork.right, philo->fork.left);
		return (1);
	}
	put_forks_down(philo->fork.right, philo->fork.left);
	return (0);
}
