/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:44:51 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/25 17:01:10 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	actions(t_philo *philo, char *action)
{
	long long	time;

	time = time_stamp() - philo->env->time;
	sem_wait(philo->env->print);
	printf("%lld %d %s\n", time, philo->index,
		action);
	sem_post(philo->env->print);
	return (0);
}

void	meal_nbr_listener(t_philo *philo)
{
	t_philo	*tmp;
	int		i;

	i = 0;
	tmp = philo;
	while (i < tmp->env->philo_num)
	{
		sem_wait(tmp->env->meal_nbr);
		i++;
	}
	while (tmp)
	{
		kill(tmp->process, 1);
		tmp = tmp->next;
	}
}

void	listener(t_philo *philo)
{
	int			i;
	long long	time;

	i = 0;
	usleep(4000);
	while (1)
	{
		if (time_stamp() - meal_method(philo, 'g', 0) > philo->env->time_to_die)
		{
			time = time_stamp() - philo->env->time;
			sem_wait(philo->env->print);
			printf("%lld %d died\n", time,
				philo->index);
			sem_post(philo->meal);
			sem_post(philo->env->mutex);
			while (i < philo->env->philo_num)
			{
				sem_post(philo->env->meal_nbr);
				i++;
			}
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
