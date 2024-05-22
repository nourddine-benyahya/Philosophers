/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:14:53 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/22 17:33:49 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	cleaning(t_philo *philo)
{
	t_philo	*tmp;

	tmp = philo;
	while (tmp)
	{
		sem_close(philo->meal);
		sem_unlink(philo->mut_name);
		tmp = tmp->next;
	}
	sem_unlink("meal_nbr");
	sem_close(philo->env->start);
	sem_unlink("start");
	sem_close(philo->env->forks);
	sem_unlink("forks");
	sem_close(philo->env->mutex);
	sem_unlink("semaphore");
	sem_close(philo->env->print);
	sem_unlink("print");
	free(philo->env);
}

int	main(int ac, char **av)
{
	t_philo		*philo;
	t_philo		*tmp;
	bool		status;
	long long	i;

	philo = parsing(ac, av);
	if (philo == NULL)
		return (1);
	i = 0;
	tmp = philo;
	status = false;
			tmp->env->time = time_stamp();
	while (tmp)
	{
		tmp->process = fork();
		if (tmp->process == 0)
		{
			sem_wait(tmp->env->start); 
			// tmp->env->time = time_stamp();
			// tmp->last_meal = time_stamp();
			pthread_create(&tmp->thread, NULL, (void *)listener, tmp);
			while (1)
			{
				actions(tmp, THINKING);
				sem_wait(tmp->env->forks);
				actions(tmp, RIGHT_FORK);
				sem_wait(tmp->env->forks);
				actions(tmp, LEFT_FORK);
				i++;
				if (tmp->env->nbr_must_eat != -1 && i == tmp->env->nbr_must_eat)
					sem_post(tmp->env->meal_nbr);
				sem_wait(philo->meal);
				tmp->last_meal = time_stamp();
				sem_post(philo->meal);
				n3ass(tmp->env->time_to_eat);
				sem_post(tmp->env->forks);
				sem_post(tmp->env->forks);
				actions(tmp, SLEEPING);
				n3ass(tmp->env->time_to_sleep);
			}
		}
		else
		{
			tmp = tmp->next;
			if (tmp == NULL)
			{

				tmp = philo;
				while (tmp)
				{
					sem_post(philo->env->start);
					tmp = tmp->next;
					usleep(40);
				}
				tmp = NULL;
			}
		}
		usleep(50);
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
