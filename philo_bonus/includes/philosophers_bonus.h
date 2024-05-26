/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:13:19 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/26 14:21:50 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include "env_bonus.h"
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
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			nbr_must_eat;
	int			nbr_must_eat_total;
	sem_t		*mutex;
	sem_t		*forks;
	sem_t		*print;
	sem_t		*meal;
	sem_t		*meal_nbr;
}				t_env;

typedef struct s_philo
{
	pthread_t	thread;
	pthread_t	thread_listner;
	pid_t		process;
	t_env		*env;
	int			index;
	char		*mut_name;
	long long	last_meal;
	sem_t		*meal;
	void		*next;
}				t_philo;

char			*ft_strdup(const char *s);
void			set_error(t_error *err, char *msg, int exit_status);
long long		time_stamp(void);
int				n3ass(long long time);
t_philo			*parsing(int ac, char **av);
char			*ft_itoa(int n);
int				actions(t_philo *philo, char *action);
void			listener(t_philo *philo);
void			meal_nbr_listener(t_philo *philo);
void			*killer(t_philo *philo);
long long		meal_method(t_philo *philo, char method, long long new_val);
void			cleaning(t_philo *philo);
void			eat(t_philo *philo, long long *i);
t_env			*open_env_sem(t_env *env);

#endif