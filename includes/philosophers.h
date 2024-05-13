/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:13:19 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/13 17:07:34 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "env.h"
# include "limits.h"
# include "pthread.h"
# include "stdbool.h"
# include "stdio.h"
# include "stdlib.h"
# include "sys/time.h"
# include "unistd.h"

# define LEFT_FORK "has taken a fork"
# define RIGHT_FORK "has taken a fork"
# define EATING "is eating"
# define DIED "died"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"

typedef struct s_error
{
	char			*msg;
	int				exit_status;
}					t_error;

typedef struct s_env
{
	int				philo_num;
	long long		time;
	long long		timer;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_must_eat;
	int				nbr_must_eat_total;
	bool			status;
	pthread_mutex_t	printing;
	pthread_mutex_t	status_mutex;
	pthread_mutex_t	timer_mutex;
}					t_env;

typedef struct s_fork
{
	pthread_mutex_t	*right;
	pthread_mutex_t	*left;
}					t_fork;

typedef struct s_philo
{
	pthread_t		thread;
	t_fork			fork;
	t_env			*env;
	long long		time_stemp;
	int				index;
	int				eating_nbr;
	long long		last_meal;
	void			*next;
	pthread_mutex_t	meal;
}					t_philo;

void				set_error(t_error *error, char *msg, int exit_status);
t_philo				*parsing(int ac, char **av);
long long			time_stamp(void);
void				philos_lister(t_philo *philo);
void				join_and_clean(t_philo *philo);
int					n3ass(long long time, t_philo *philos);
int					actions(t_philo *philos, char *action);
int					put_forks_down(pthread_mutex_t *right,
						pthread_mutex_t *left);
int					pickup_forks(t_philo *philo);
int					eat(t_philo *philo);
int					_sleep(t_philo *philo);

#endif