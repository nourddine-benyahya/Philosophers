/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:14:53 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/23 21:13:33 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	routine(t_philo *philo)
{
	long long	i;

	i = 0;
	pthread_create(&philo->thread, NULL, (void *)listener, philo);
	if (philo->index % 2 == 0)
	{
		actions(philo, SLEEPING, 0);
		n3ass(philo->env->time_to_sleep);
	}
	while (1)
	{
		actions(philo, THINKING, 0);
		eat(philo, &i);
		actions(philo, SLEEPING, 0);
		n3ass(philo->env->time_to_sleep);
	}
}

int	main(int ac, char **av)
{
	t_philo		*philo;
	t_philo		*tmp;

	philo = parsing(ac, av);
	if (philo == NULL)
		return (1);
	tmp = philo;
	tmp->env->time = time_stamp();
	while (tmp)
	{
		tmp->process = fork();
		if (tmp->process == 0)
			routine(tmp);
		else
			tmp = tmp->next;
	}
	tmp = philo;
	pthread_create(&tmp->thread, NULL, (void *)meal_nbr_listener, tmp);
	tmp = philo;
	killer(tmp);
	tmp = philo;
	while (tmp)
	{
		waitpid(tmp->process, NULL, 0);
		tmp = tmp->next;
	}
	cleaning(philo);
	return (0);
}
