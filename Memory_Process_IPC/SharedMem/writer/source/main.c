#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#include <stdint.h>
#include <stdbool.h>

#define SHM_NAME "/my_shm_struct"
#define MUTEX_NAME "/my_mutex_sem"
#define DATA_READY_NAME "/my_data_sem"
#define SIZE sizeof(struct SharedData)

struct SharedData
{
    uint16_t ID;
    bool state;
    char desc[100];
};

int main(int argc,char *argv[])
{
    int shm_fd;
    struct SharedData *shared;
    sem_t *mutex,*data_ready;

    //open shared mem
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        /* code */
        perror("shm_open");
        exit(0);
    }
    
    //set size
    ftruncate(shm_fd, SIZE);

    //map shared memory
    shared = mmap(0,SIZE,PROT_WRITE,MAP_SHARED,shm_fd,0);
    if (shared == MAP_FAILED)
    {
        /* code */
        perror("mmap");
        exit(0);
    }

    //open semaphore
    mutex = sem_open(MUTEX_NAME,0);
    data_ready = sem_open(DATA_READY_NAME,0);

    if (mutex == SEM_FAILED || data_ready == SEM_FAILED)
    {
        /* code */
        perror("sem_opem");
        exit(0);
    }

    // critial section
    sem_wait(mutex);

    shared->state = true;
    shared->ID = 1;
    strcpy(shared->desc,"Process");

    sem_post(mutex);

    //signal to reader
    sem_post(data_ready);

    // clean up
    munmap(shared,SIZE);
    close(shm_fd);

    return 0;
}
