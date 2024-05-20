int main()
{
    sem_close(philo->env->start);
	sem_unlink("start");
	sem_close(philo->env->forks);
	sem_unlink("forks");
	sem_close(philo->env->mutex);
	sem_unlink("semaphore");
	sem_close(philo->env->print);
	sem_unlink("print");
}