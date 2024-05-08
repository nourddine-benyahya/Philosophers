/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:18:06 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/08 17:19:53 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *nptr)
{
	unsigned long long	res;

	res = 0;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			exit_with_msg(YELHB BRED URED "\n huh it's negative value\n" reset, 1);
		nptr++;
	}
	while (*nptr && (*nptr) >= '0' && (*nptr) <= '9')
	{
		res = (res * 10) + (*nptr - '0');
		if (res > INT_MAX)
			exit_with_msg(YELHB BRED URED "\n it's more than int max\n" reset, 1);
		nptr++;
	}
	return (res);
}



t_philo	*parsing(int ac, char **av)
{
    t_env	*env = NULL;
    t_philo	*philo;
    t_philo *head = NULL;
    t_philo *prev = NULL;

    if (ac != 6 && ac != 5)
        exit_with_msg(YELHB BRED URED "\n the number of arg not valid \n" reset, 1);
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
    env->time = time_stamp();
    env->philo_num = ft_atoi(av[1]);
	env->status = true;
    env->time_to_die = ft_atoi(av[2]);
    env->time_to_eat = ft_atoi(av[3]);
    env->time_to_sleep = ft_atoi(av[4]);
    env->printing = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (ac == 6)
        env->nbr_must_eat = ft_atoi(av[5]);
    if (env->philo_num > 200 || env->time_to_die < 60 || env->time_to_eat < 60 
        || env->time_to_sleep < 60)
        exit_with_msg(YELHB BRED URED "\n the info it's not correct \n" reset, 1);
	
	
 // create a circular linked list
    int i = 0;
    while (i < env->philo_num)
    {
        philo = (t_philo*)malloc(sizeof(t_philo));
        philo->index = i;
        philo->eating_nbr = 0;
        philo->env = env;
		philo->last_meal = time_stamp();
        philo->fork = (t_fork*)malloc(sizeof(t_fork));
        philo->fork->right = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        if (pthread_mutex_init(philo->fork->right, NULL) != 0)
            exit_with_msg(YELHB BRED URED "\n mutex init has failed\n" reset, 1);
        if (i == 0)
        {
            head = philo;
        }
        else
        {
            prev->next = philo;
            philo->fork->left = prev->fork->right;
        }
        prev = philo;
        i++;
    }
    // Connect the last philosopher to the first one
    head->fork->left = prev->fork->right;
    prev->next = NULL;

    return (head);
}
