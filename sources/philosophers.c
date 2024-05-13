/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:14:53 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/13 17:51:55 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *philo)
{
	bool	status;

	status = true;
	((t_philo *)philo)->last_meal = time_stamp();
	if (((t_philo *)philo)->index % 2 == 0)
	{
		if (actions((t_philo *)philo, THINKING) == 1)
			return (NULL);
		n3ass(10, (t_philo *)philo);
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
		pthread_create(&tmp->thread, NULL, &routine, tmp);
		tmp = tmp->next;
	}
	philos_lister(philo);
	join_and_clean(philo);
	return (0);
}
