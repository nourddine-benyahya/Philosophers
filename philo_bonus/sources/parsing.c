/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:18:06 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/24 12:27:24 by nbenyahy         ###   ########.fr       */
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
			return (set_error(error, REDHB "huh it's negative value\n" RESET,
					1), 1);
		nptr++;
	}
	while (*nptr && (*nptr) >= '0' && (*nptr) <= '9')
	{
		res = (res * 10) + (*nptr - '0');
		if (res > INT_MAX)
			return (set_error(error, REDHB "it's more than int max\n" RESET, 1),
				1);
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
	sem_unlink(philo->mut_name);
	philo->meal = sem_open(philo->mut_name, O_CREAT | O_EXCL, 0777, 1);
	if (philo->meal == SEM_FAILED)
		return (NULL);
	philo->last_meal = env->time;
	philo->index = i + 1;
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
		return (printf(REDHB "the info it's not correct \n" RESET),
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
	if (!env)
		return (NULL);
	if (!open_env_sem(env))
		return (NULL);
	if (env == NULL || env->nbr_must_eat == 0)
		return (NULL);
	philo = create_philos(env);
	if (philo == NULL)
		return (free(env), NULL);
	return (philo);
}
