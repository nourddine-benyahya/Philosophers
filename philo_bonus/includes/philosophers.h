/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:13:19 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/23 21:11:22 by nbenyahy         ###   ########.fr       */
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
# define RIGHT_FORK "has taken a fork"
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
	sem_t		*meal_nbr;
	sem_t		*init_time;
	bool		status;
}				t_env;

typedef struct s_philo
{
	pthread_t	thread;
	pthread_t	thread_listner;
	pid_t		process;
	t_env		*env;
	long long	time_stemp;
	int			index;
	char		*mut_name;
	int			eating_nbr;
	long long	last_meal;
	sem_t		*meal;
	sem_t		*starting;
	char		*starting_name;
	void		*next;
}				t_philo;

char			*ft_strdup(const char *s);
void			set_error(t_error *, char *, int);
long long		time_stamp(void);
int				n3ass(long long time);
t_philo			*parsing(int ac, char **av);
char			*ft_itoa(int n);
void			kill_process(t_philo *philo);
int				actions(t_philo *philo, char *action, int );
void			listener(t_philo *philo);
void			meal_nbr_listener(t_philo *philo);
void			*killer(t_philo *philo);
long long	meal_method(t_philo *philo, char method, long long new_val);
void	cleaning(t_philo *philo);
void eat(t_philo *philo, long long *i);

#endif