/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:44:51 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/25 19:26:03 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	sleeping(long long time, t_philo *philos)
{
	bool		status;
	long long	timer;

	status = true;
	timer = time_stamp();
	while (status)
	{
		if (time_stamp() - timer >= time)
			break ;
		status = status_method(philos, 'g', NULL);
		usleep(50);
	}
	return (0);
}

int	actions(t_philo *philos, char *action)
{
	int	status;

	pthread_mutex_lock(&philos->env->printing);
	if (status_method(philos, 'g', NULL))
	{
		printf("%lld %d %s\n", time_stamp() - philos->env->time, \
				philos->index, action);
		status = 0;
	}
	else
		status = 1;
	pthread_mutex_unlock(&philos->env->printing);
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
		sleeping(philo->env->time_to_die, philo);
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
	meal_method(philo, 's', time_stamp());
	if (actions(philo, EATING) == 1)
	{
		put_forks_down(philo->fork.right, philo->fork.left);
		return (1);
	}
	if (sleeping(philo->env->time_to_eat, philo) == 1)
	{
		put_forks_down(philo->fork.right, philo->fork.left);
		return (1);
	}
	put_forks_down(philo->fork.right, philo->fork.left);
	if (philo->env->nbr_must_eat != -1)
	{
		if (philo->eating_nbr != philo->env->nbr_must_eat)
		{
			philo->eating_nbr++;
			pthread_mutex_lock(&philo->env->nbr_must_eat_total_mutex);
			philo->env->nbr_must_eat_total-- ;
			pthread_mutex_unlock(&philo->env->nbr_must_eat_total_mutex);
		}
	}
	if (meal_nbr_total(philo, 'g', 0) == 0)
		status_method(philo, 's', false);
	return (0);
}
