/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:14:53 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/22 17:44:24 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philos_lister(t_philo *philo, t_philo *head)
{
	bool	status;

	status = true;
	while (status)
	{
		if (time_stamp() - meal_method(philo, 'g', 0) > philo->env->time_to_die)
		{
			status_method(philo, 's', false);
			pthread_mutex_lock(&philo->env->printing);
			printf("%lld %d %s\n", time_stamp() - philo->env->time, philo->index,
				DIED);
			pthread_mutex_unlock(&philo->env->printing);
		}
		philo = philo->next;
		if (philo == NULL)
			philo = head;
		status = status_method(philo, 'g', NULL);
	}
}

void	*routine(void *philo)
{
	bool	status;

	status = true;
	if (((t_philo *)philo)->index % 2 == 0)
		_sleep((t_philo *)philo);
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
		status = status_method(((t_philo *)philo), 'g', NULL);
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
	tmp->env->time = time_stamp();
	while (tmp)
	{
		if ((pthread_create(&tmp->thread, NULL, &routine, tmp)))
			break ;
		tmp = tmp->next;
	}
	tmp = philo;
	philos_lister(tmp, philo);
	join_and_clean(philo);
	return (0);
}
