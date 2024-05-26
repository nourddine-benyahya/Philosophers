/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:20:52 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/26 14:19:31 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	n3ass(long long time)
{
	long long	timer;

	timer = time_stamp();
	while (1)
	{
		if (time_stamp() - timer >= time)
			break ;
		usleep(50);
	}
	return (0);
}

char	*ft_strdup(const char *s)
{
	char	*ptr;
	size_t	i;
	size_t	len;

	len = 0;
	while (s[len++])
		;
	ptr = (char *)malloc(sizeof(char) * (--len + 1));
	if (!ptr)
	{
		return (NULL);
	}
	i = 0;
	while (s[i])
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

void	set_error(t_error *error, char *msg, int exit_status)
{
	if (error->exit_status == 0)
	{
		error->msg = ft_strdup(msg);
		error->exit_status = exit_status;
	}
}

void	cleaning(t_philo *philo)
{
	t_philo	*tmp;

	sem_close(philo->env->forks);
	sem_unlink("forks");
	sem_close(philo->env->mutex);
	sem_unlink("semaphore");
	sem_close(philo->env->print);
	sem_unlink("print");
	sem_close(philo->env->meal);
	sem_unlink("meal");
	sem_close(philo->env->meal_nbr);
	sem_unlink("meal_nbr");
	free(philo->env);
	tmp = philo;
	while (philo)
	{
		tmp = philo;
		sem_close(philo->meal);
		sem_unlink(philo->mut_name);
		free(philo->mut_name);
		philo = philo->next;
		free(tmp);
	}
}

t_env	*open_env_sem(t_env *env)
{
	sem_unlink("semaphore");
	sem_unlink("forks");
	sem_unlink("print");
	sem_unlink("meal");
	sem_unlink("meal_nbr");
	env->mutex = sem_open("semaphore", O_CREAT | O_EXCL, 0777, 0);
	env->forks = sem_open("forks", O_CREAT | O_EXCL, 0777, env->philo_num);
	env->print = sem_open("print", O_CREAT | O_EXCL, 0777, 1);
	env->meal = sem_open("meal", O_CREAT | O_EXCL, 0777, 1);
	env->meal_nbr = sem_open("meal_nbr", O_CREAT | O_EXCL, 0777, 0);
	if (env->meal_nbr == SEM_FAILED || env->forks == SEM_FAILED 
		|| env->mutex == SEM_FAILED || env->print == SEM_FAILED 
		|| env->meal == SEM_FAILED)
		return (free(env), NULL);
	env->nbr_must_eat_total = env->nbr_must_eat * env->philo_num;
	env->time = time_stamp();
	return (env);
}
