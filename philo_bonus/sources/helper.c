/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:20:52 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/19 17:49:18 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	n3ass(long long time, t_philo *philo)
{
	long long	timer;
	// int status = 0;
	(void)philo;

	timer = time_stamp();
	while (1)
	{
		if (time_stamp() - timer >= time)
			break ;
		// sem_wait(philo->meal);
		// if (time_stamp() - philo->last_meal > philo->env->time_to_die)
		// {
		// 	sem_post(philo->env->mutex);
		// 	sem_post(philo->meal);
		// 	// sem_wait(philo->env->print);
		// 	status = 1;
		// 	// break;
		// }
		// else
		// 	sem_post(philo->meal);
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

long long	time_stamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
