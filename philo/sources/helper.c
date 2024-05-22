/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:20:52 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/22 16:41:24 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	join_and_clean(t_philo *philo)
{
	t_philo	*tmp;

	tmp = philo;
	while (tmp)
	{
		pthread_join(tmp->thread, NULL);
		tmp = tmp->next;
	}
	free(philo->env);
	while (philo)
	{
		tmp = philo;
		philo = philo->next;
		free(tmp->fork.right);
		free(tmp);
	}
}

long long	time_stamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	_sleep(t_philo *philo)
{
	if (actions(philo, SLEEPING) == 1)
		return (1);
	if (sleeping(philo->env->time_to_sleep, philo) == 1)
		return (1);
	return (0);
}
