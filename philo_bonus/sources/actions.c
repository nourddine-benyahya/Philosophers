/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:44:51 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/23 21:21:46 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	actions(t_philo *philo, char *action, int a)
{
	if (a == 1)
		meal_method(philo, 's', time_stamp());
	sem_wait(philo->env->print);
	printf("%lld %d %s\n", time_stamp() - philo->env->time, philo->index,
		action);
	if (a == 1)
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
	// sem_wait(tmp->env->print);
	while (tmp)
	{
		kill(tmp->process, 1);
		tmp = tmp->next;
	}
}

void	listener(t_philo *philo)
{
	usleep(5000);
	while (1)
	{
		if (time_stamp() - meal_method(philo, 'g', 0) > philo->env->time_to_die)
		{
			sem_wait(philo->env->print);
			printf("%lld %d died\n", time_stamp() - philo->env->time,
				philo->index);
			sem_post(philo->meal);
			sem_post(philo->env->mutex);
		}
		usleep(1000);
	}
}

void	*killer(t_philo *philo)
{
	if (philo->env->nbr_must_eat != -1)
		return (NULL);
	sem_wait(philo->env->mutex);
	while (philo)
	{
		kill(philo->process, 1);
		philo = philo->next;
	}
	return (NULL);
}
