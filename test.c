#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>

#define NUM_PROCESSES 10
#define NUM_FIRST_PROCESSES 5
#define SEMAPHORE_NAME "/example_semaphoreeeeeee"

int main() {
    sem_t *semaphore = sem_open(SEMAPHORE_NAME, O_CREAT | O_EXCL, 0644, 5); // Create and initialize semaphore

    if (semaphore == SEM_FAILED) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        pid_t pid = fork();

        if (pid == -1) 
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0) 
        { // Child process
            int process_id = i;
            printf("waiting %d\n", process_id);
            sem_wait(semaphore); 
            printf("Process %d: Printing immediately.\n", process_id);
            usleep(5000000);
            sem_post(semaphore);
            printf("done %d\n", process_id);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        wait(NULL);
    }
    // Wait for all child processes to finish
    sem_close(semaphore); // Close semaphore
    sem_unlink(SEMAPHORE_NAME); // Remove semaphore from system

    return 0;
}
