/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:18:06 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/20 17:26:51 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *nptr, t_error *error)
{
	unsigned long long	res;

	res = 0;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			return (set_error(error, \
								REDHB "huh it's negative value\n" RESET, 1), 1);
		nptr++;
	}
	while (*nptr && (*nptr) >= '0' && (*nptr) <= '9')
	{
		res = (res * 10) + (*nptr - '0');
		if (res > INT_MAX)
			return (set_error(error, \
								REDHB "it's more than int max\n" RESET, 1), 1);
		nptr++;
	}
	if (*nptr)
		return (set_error(error, REDHB "it's not a number\n" RESET, 1), 1);
	return (res);
}

t_philo	*init_philo(t_env *env, int i)
{
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->env = env;
	philo->mut_name = ft_itoa(i + 1);
	// philo->meal_mut_name = ft_itoa((i + 1) * -1);
	philo->meal = sem_open(philo->mut_name, O_CREAT | O_EXCL, 0644, 1);
	// philo->meal_nbr = sem_open(philo->meal_mut_name, O_CREAT | O_EXCL, 0644, 0);
	philo->last_meal = env->time;
	philo->index = i + 1;
	philo->eating_nbr = 0;
	return (philo);
}

t_env	*create_env(int ac, char **av)
{
	t_env	*env;
	t_error	*error;

	env = malloc(sizeof(t_env));
	error = malloc(sizeof(t_error));
	if (!env || !error)
		return (NULL);
	error->exit_status = 0;
	error->msg = NULL;
	env->philo_num = ft_atoi(av[1], error);
	env->time_to_die = ft_atoi(av[2], error);
	env->time_to_eat = ft_atoi(av[3], error);
	env->time_to_sleep = ft_atoi(av[4], error);
	if (ac == 6)
		env->nbr_must_eat = ft_atoi(av[5], error);
	else
		env->nbr_must_eat = -1;
	if (error->exit_status == 1)
		return (printf("%s", error->msg), free(error->msg), free(error),
			free(env), NULL);
	if (env->philo_num > 200 || env->time_to_die < 60 || env->time_to_eat < 60
		|| env->time_to_sleep < 60)
		return (printf(REDHB "the info it's not correct \n" RESET), \
						free(error->msg), free(error), free(env), NULL);
	return (free(error->msg), free(error), env);
}

t_philo	*create_philos(t_env *env)
{
	t_philo	*philo;
	t_philo	*head;
	t_philo	*prev;
	int		i;

	prev = NULL;
	head = NULL;
	i = 0;
	while (i < env->philo_num)
	{
		philo = init_philo(env, i);
		if (philo == NULL)
			return (NULL);
		if (i == 0)
			head = philo;
		else
			prev->next = philo;
		prev = philo;
		i++;
	}
	return (prev->next = NULL, head);
}

t_philo	*parsing(int ac, char **av)
{
	t_philo	*philo;
	t_env	*env;

	if (ac != 6 && ac != 5)
		return (printf(REDHB "the number of arg not valid \n" RESET), NULL);
	env = create_env(ac, av);
	if (env == NULL || env->nbr_must_eat == 0)
		return (NULL);
	env->status = true;
	env->timer = time_stamp();
	env->mutex = sem_open("semaphore", O_CREAT | O_EXCL, 0644, 0);
	env->forks = sem_open("forks", O_CREAT | O_EXCL, 0644, env->philo_num);
	env->print = sem_open("print", O_CREAT | O_EXCL, 0644, 1);
	env->meal = sem_open("meal", O_CREAT | O_EXCL, 0644, 1);
	env->start = sem_open("start", O_CREAT | O_EXCL, 0644, 0);
	env->init_time = sem_open("start", O_CREAT | O_EXCL, 0644, 0);
	env->meal_nbr = sem_open("meal_nbr", O_CREAT | O_EXCL, 0644, 0);
	env->nbr_must_eat_total = env->nbr_must_eat * env->philo_num;
	env->time = time_stamp();
	philo = create_philos(env);
	if (philo == NULL)
		return (free(env), NULL);
	// t_philo *tmp =philo;
	// while (tmp)
	// {
	// 	sem_close(philo->meal);
	// 	sem_unlink(philo->mut_name);
	// 	// free(philo->mut_name);
	// 	// pthread_join(tmp->thread, NULL);
	// 	tmp = tmp->next;
	// }
	// sem_close(philo->env->mutex);
	// sem_unlink("semaphore");
	// sem_close(philo->env->print);
	// sem_unlink("print");
	// exit(1);
	return (philo);
}
