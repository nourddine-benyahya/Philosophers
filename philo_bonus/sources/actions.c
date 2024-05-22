/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:44:51 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/21 11:03:30 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	actions(t_philo *philo, char *action)
{
	sem_wait(philo->env->print);
	printf("%lld %d %s\n", time_stamp() - philo->env->time, philo->index,
		action);
	if (action == LEFT_FORK)
		printf("%lld %d %s\n", time_stamp() - philo->env->time, philo->index,
			EATING);
	sem_post(philo->env->print);
	return (0);
}

void	meal_nbr_listener(t_philo *philo)
{
	t_philo	*tmp;

	tmp = philo;
	while (tmp)
	{
		sem_wait(tmp->env->meal_nbr);
		tmp = tmp->next;
	}
	tmp = philo;
	sem_wait(tmp->env->print);
	while (tmp)
	{
		kill(tmp->process, 1);
		tmp = tmp->next;
	}
}

void	listener(t_philo *philo)
{
	while (1)
	{
		sem_wait(philo->meal);
		if (time_stamp() - philo->last_meal > philo->env->time_to_die)
		{
			// printf("%lld--> %d\n",time_stamp() - philo->last_meal , philo->env->time_to_die);
			sem_post(philo->meal);
			sem_post(philo->env->mutex);
		}
		else
			sem_post(philo->meal);
		usleep(1000);
	}
}

void	*killer(t_philo *philo)
{
	if (philo->env->nbr_must_eat != -1)
		return (NULL);
	sem_wait(philo->env->mutex);
	sem_wait(philo->env->print);
	printf("%lld %d died\n", time_stamp() - philo->env->time, philo->index);
	while (philo)
	{
		kill(philo->process, 1);
		philo = philo->next;
	}
	return (NULL);
}
