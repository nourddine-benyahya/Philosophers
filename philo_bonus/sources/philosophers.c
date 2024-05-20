/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:14:53 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/20 17:31:40 by nbenyahy         ###   ########.fr       */
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
	bool	meal_status;
	long long i = 0;


	philo = parsing(ac, av);
	if (philo == NULL)
		return (1);
	philo->head = philo;
	tmp = philo;
	status = false;
	if (philo->env->nbr_must_eat != -1)
		meal_status = false;
	else
		meal_status = true;
	while (tmp)
	{
		tmp->process = fork();
		if (tmp->process == 0)
		{
			actions(tmp, THINKING);
			sem_wait(tmp->env->start);
			pthread_create(&tmp->thread, NULL, (void *)listener, tmp);
			while (1)
			{
				if (status)
					actions(tmp, THINKING);
				status = true;

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
	pthread_create(&tmp->thread, NULL, (void *)meal_nbr_listener, tmp);
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
		tmp = tmp->next;
	}
	sem_close(philo->env->meal_nbr);
	sem_unlink("meal_nbr");
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
