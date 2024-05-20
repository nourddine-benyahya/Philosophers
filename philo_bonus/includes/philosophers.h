/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:13:19 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/20 11:48:50 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "env.h"
# include "limits.h"
# include "pthread.h"
# include "semaphore.h"
# include "stdbool.h"
# include "stdio.h"
# include "stdlib.h"
# include "sys/time.h"
# include "unistd.h"
# include <signal.h>

# define LEFT_FORK "has taken a fork"
# define RIGHT_FORK "has taken a fork "
# define EATING "is eating"
# define DIED "died"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"

typedef struct s_error
{
	char		*msg;
	int			exit_status;
}				t_error;

typedef struct s_env
{
	int			philo_num;
	long long	time;
	long long	timer;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			nbr_must_eat;
	int			nbr_must_eat_total;
	sem_t		*mutex;
	sem_t		*start;
	sem_t		*forks;
	sem_t		*print;
	sem_t		*meal;
	sem_t		*init_time;
	bool		status;
}				t_env;

typedef struct s_philo
{
	pthread_t	thread;
	pthread_t	thread_listner;
	void		*head;
	pid_t		process;
	t_env		*env;
	long long	time_stemp;
	int			index;
	char		*mut_name;
	int			eating_nbr;
	long long	last_meal;
	sem_t		*meal;
	void		*next;
}				t_philo;

char			*ft_strdup(const char *s);
void			set_error(t_error *, char *, int);
long long		time_stamp(void);
int				n3ass(long long time, t_philo *philo);
t_philo			*parsing(int ac, char **av);
char			*ft_itoa(int n);
void			kill_process(t_philo *philo);

#endif