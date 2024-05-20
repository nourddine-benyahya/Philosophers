/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:14:53 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/20 12:00:43 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


int actions(t_philo *philo, char *action)
{
	sem_wait(philo->env->print);
	printf("%lld %d %s\n", time_stamp() - philo->env->time, philo->index, action);
	if (action == LEFT_FORK)
		printf("%lld %d %s\n", time_stamp() - philo->env->time, philo->index, EATING);
	sem_post(philo->env->print);
	return (0);
}

void	listener(t_philo *philo)
{
	while (1)
	{
		sem_wait(philo->meal);
		if (time_stamp() - philo->last_meal > philo->env->time_to_die)
		{
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
	sem_wait(philo->env->mutex);
	sem_wait(philo->env->print);
	printf("%lld %d died\n", time_stamp() - philo->env->time ,philo->index);
	while (philo)
	{
		kill(philo->process, 1);
		philo = philo->next;
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_philo	*philo;
	t_philo	*tmp;
	bool	status;

	philo = parsing(ac, av);
	if (philo == NULL)
		return (1);
	philo->head = philo;
	tmp = philo;
	status = false;
	while (tmp)
	{
		tmp->process = fork();
		if (tmp->process == 0)
		{
			// sem_wait(tmp->env->init_time);
			// tmp->last_meal = time_stamp();
			// tmp->env->time = time_stamp();
			// if (tmp->index %2 == 0)
			// 	usleep(50);
			actions(tmp, THINKING);
			sem_wait(tmp->env->start);
			pthread_create(&tmp->thread, NULL, (void *)listener , tmp);
			tmp->last_meal = time_stamp();
			// tmp->env->time = time_stamp();
			while (1)
			{
				if (status)
					actions(tmp, THINKING);
				status = true;

				sem_wait(tmp->env->forks);
				actions(tmp, RIGHT_FORK);

				sem_wait(tmp->env->forks);
				actions(tmp, LEFT_FORK);

				sem_wait(philo->meal);
				tmp->last_meal = time_stamp();
				sem_post(philo->meal);

				n3ass(tmp->env->time_to_eat, tmp);

				sem_post(tmp->env->forks);
				sem_post(tmp->env->forks);

				actions(tmp, SLEEPING);

				n3ass(tmp->env->time_to_sleep, tmp);
			}
		}
		else
		{
			tmp = tmp->next;
			if (tmp == NULL)
			{
				// tmp = philo;
				// while (tmp)
				// {
				// 	sem_post(philo->env->init_time);
				// 	tmp = tmp->next;
				// }
				tmp = philo;
				while (tmp)
				{
					sem_post(philo->env->start);
					tmp = tmp->next;
					usleep(100);
				}
				tmp = NULL;
			}
		}
	}

	tmp = philo;
	killer(tmp);
	tmp = philo;
	while (tmp)
	{
    	waitpid(tmp->process, NULL, 0);
    	tmp = tmp->next;
	}
	while (tmp)
	{
		sem_close(philo->meal);
		sem_unlink(philo->mut_name);
		pthread_join(tmp->thread, NULL);
		tmp = tmp->next;
	}
	sem_close(philo->env->start);
	sem_unlink("start");
	sem_close(philo->env->forks);
	sem_unlink("forks");
	sem_close(philo->env->mutex);
	sem_unlink("semaphore");
	sem_close(philo->env->print);
	sem_unlink("print");
	printf("done\n");
	return (0);
}
