/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:14:53 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/08 18:14:52 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include "unistd.h"
#include "stdio.h"
#include "pthread.h"

long long time_stamp(void)
{
   struct timeval tv;
   
   gettimeofday(&tv, NULL);
   return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int  n3ass(long long time, t_philo *philos)
{
   // (void )philos;
   long long timer = time_stamp();
   while (1)
   {
      if (time_stamp() - timer >= time)
         break;
      if (philos->env->status == false)
         return (1);
   }
   return (0);
}

void *roting(void *philo)
{
   t_philo *philos;

   philos = (t_philo *)philo;
   if ((philos->index + 1) % 2 == 0)
      n3ass(50, philos);

   while (philos->env->status == true)
   {

      //think while waiting for fork
          if (philos->env->status == true)
      {
      pthread_mutex_lock(philos->env->printing);
      printf(BGRN UGRN "%lld philo %d is thinking\n" reset, time_stamp() - philos->env->time, philos->index);
      pthread_mutex_unlock(philos->env->printing);
      }
      
      //take the right fort
      pthread_mutex_lock(philos->fork->right);
          if (philos->env->status == true)
      {
      pthread_mutex_lock(philos->env->printing);
      printf(BGRN UGRN "%lld philo %d is taking a right fork\n" reset, time_stamp() - philos->env->time, philos->index);
      pthread_mutex_unlock(philos->env->printing);
      }

      //take the left fork
      pthread_mutex_lock(philos->fork->left);
    if (philos->env->status == true)
      {
      pthread_mutex_lock(philos->env->printing);
      printf(BGRN UGRN "%lld philo %d is taking a left fork\n" reset, time_stamp() - philos->env->time, philos->index);
      pthread_mutex_unlock(philos->env->printing);
      }

      //eating time
      philos->last_meal = time_stamp();
    if (philos->env->status == true)
      {
      pthread_mutex_lock(philos->env->printing);
      printf(BBLU UBLU "%lld philo %d is eating\n" reset, time_stamp() - philos->env->time, philos->index);
      pthread_mutex_unlock(philos->env->printing);
      }

      if (n3ass(philos->env->time_to_eat, philos) == 1)
      {
         pthread_mutex_unlock(philos->fork->right);
         pthread_mutex_unlock(philos->fork->left);
         break;
      }

      pthread_mutex_unlock(philos->fork->right);
      pthread_mutex_unlock(philos->fork->left);

      //sleeping time
      if (philos->env->status == true)
      {
         pthread_mutex_lock(philos->env->printing);
         printf(BGRN UGRN "%lld philo %d is sleeping\n" reset, time_stamp() - philos->env->time, philos->index);
         pthread_mutex_unlock(philos->env->printing);   
      }

      if (n3ass(philos->env->time_to_sleep, philos) == 1)
      {
         break;
      }
      // printf("q\n");
   }
   return (NULL);
}

int main(int ac, char **av)
{
   t_philo *philo;
   t_philo *head;

   philo = parsing(ac, av);
   head = philo;
   while (head)
   {
      pthread_create(&head->thread, NULL, &roting, head);
      head = head->next;
   }
   head = philo;
   while (head->env->status)
   {
      if (time_stamp() - head->last_meal > head->env->time_to_die)
      {
         head->env->status = false;
         usleep(500);
         pthread_mutex_lock(head->env->printing);
         printf(MAGHB BGRN UGRN "\n\v %lld philo %d is deaing take %lld\n" reset, time_stamp() - head->env->time , head->index, time_stamp() - head->last_meal);
         pthread_mutex_unlock(head->env->printing);

         break;
      }
      head = head->next;
      if (head == NULL)
         head = philo;
   }
   while (philo)
   {
      pthread_join(philo->thread, NULL);
      philo= philo->next;
   }
}
