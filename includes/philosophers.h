/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:13:19 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/08 17:19:44 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "env.h"
# include "stdio.h"
# include "limits.h"
# include "stdlib.h"
# include "pthread.h"
# include "stdbool.h"
# include "sys/time.h"

typedef struct s_env
{
    int         philo_num;
    long long   time;
    int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
    int         nbr_must_eat;
    bool        status;
    pthread_mutex_t         *printing;
} t_env;

typedef struct s_fork
{
    pthread_mutex_t *right;
    pthread_mutex_t *left;
}   t_fork;

typedef struct s_philo
{
    pthread_t   thread;
    t_fork      *fork;
    t_env       *env;
    int         index;
    int         eating_nbr;
    long long   last_meal;
    void        *next;
}   t_philo;

void exit_with_msg(char *msg, int status);
t_philo	*parsing(int ac, char **av);
long long time_stamp(void);

#endif