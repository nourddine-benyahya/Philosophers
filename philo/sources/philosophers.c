/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:14:53 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/19 18:13:11 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	_sleep(t_philo *philo)
{
	if (actions(philo, SLEEPING) == 1)
		return (1);
	if (n3ass(philo->env->time_to_sleep, philo) == 1)
		return (1);
	return (0);
}

void	philos_lister(t_philo *philo, t_philo *head)
{
	bool	status;

	status = true;
	while (status)
	{
		pthread_mutex_lock(&philo->meal);
		if (time_stamp() - philo->last_meal > philo->env->time_to_die)
		{
			pthread_mutex_lock(&philo->env->status_mutex);
			philo->env->status = false;
			pthread_mutex_unlock(&philo->env->status_mutex);
			pthread_mutex_lock(&philo->env->printing);
			printf("%lld %d %s\n", time_stamp() - philo->env->time, philo->index,
				DIED);
			pthread_mutex_unlock(&philo->env->printing);
		}
		pthread_mutex_unlock(&philo->meal);
		philo = philo->next;
		if (philo == NULL)
			philo = head;
		pthread_mutex_lock(&philo->env->status_mutex);
		status = philo->env->status;
		pthread_mutex_unlock(&philo->env->status_mutex);
	}
}

void	*routine(void *philo)
{
	bool	status;

	status = true;
	if (((t_philo *)philo)->index % 2 == 0)
	{
		if (actions((t_philo *)philo, THINKING) == 1)
			return (NULL);
		n3ass(5, (t_philo *)philo);
	}
	while (status)
	{
		if (actions((t_philo *)philo, THINKING) == 1)
			break ;
		if (pickup_forks((t_philo *)philo) == 1)
			break ;
		if (eat((t_philo *)philo) == 1)
			break ;
		if (_sleep((t_philo *)philo) == 1)
			break ;
		pthread_mutex_lock(&((t_philo *)philo)->env->status_mutex);
		status = ((t_philo *)philo)->env->status;
		pthread_mutex_unlock(&((t_philo *)philo)->env->status_mutex);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_philo	*philo;
	t_philo	*tmp;

	philo = parsing(ac, av);
	if (philo == NULL)
		return (1);
	tmp = philo;
	while (tmp)
	{
		tmp->last_meal = time_stamp();
		tmp->env->time = time_stamp();
		pthread_create(&tmp->thread, NULL, &routine, tmp);
		tmp = tmp->next;
	}
	tmp = philo;
	philos_lister(tmp, philo);
	join_and_clean(philo);
	return (0);
}
