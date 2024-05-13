/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:20:52 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/13 17:55:13 by nbenyahy         ###   ########.fr       */
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

void	philos_lister(t_philo *philo)
{
	t_philo	*tmp;
	bool	status;

	status = true;
	tmp = philo;
	while (status)
	{
		pthread_mutex_lock(&tmp->meal);
		if (time_stamp() - tmp->last_meal > tmp->env->time_to_die)
		{
			pthread_mutex_lock(&tmp->env->status_mutex);
			tmp->env->status = false;
			pthread_mutex_unlock(&tmp->env->status_mutex);
			pthread_mutex_lock(&tmp->env->printing);
			printf("%lld %d %s\n", time_stamp() - tmp->env->time, tmp->index,
				DIED);
			pthread_mutex_unlock(&tmp->env->printing);
		}
		pthread_mutex_unlock(&tmp->meal);
		tmp = tmp->next;
		if (tmp == NULL)
			tmp = philo;
		pthread_mutex_lock(&tmp->env->status_mutex);
		status = tmp->env->status;
		pthread_mutex_unlock(&tmp->env->status_mutex);
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
