#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#include <stdint.h>
#include <stdbool.h>

#define SHM_NAME "/my_shm_struct"
#define MUTEX_NAME "/my_mutex_sem"
#define DATA_READY_NAME "/my_data_sem"

#define SIZE sizeof(struct SharedData)

struct SharedData
{
    /* data */
    uint16_t id;
    bool state;
    char desc[100];
};

int main(int argc,char *argv[])
{
    int shm_fd;
    struct SharedData *shared;
    sem_t *mutex,*data_ready;

    // open shared memory
    shm_fd = shm_open(SHM_NAME,O_CREAT | O_RDONLY, 0666);
    if(shm_fd == -1)
    {
        perror("shm open");
        exit(1);
    }

    // set size shared memory
    ftruncate(shm_fd, SIZE);

    // map shared memory
    shared = mmap(0,SIZE,PROT_READ,MAP_SHARED,shm_fd,0);
    if (shared == MAP_FAILED)
    {
        /* code */
        perror("mmap");
        exit(0);
    }

    //open semaphore
    mutex = sem_open(MUTEX_NAME,O_CREAT,0666,1);
    data_ready = sem_open(DATA_READY_NAME,O_CREAT,0666,0);

    if (mutex == SEM_FAILED || data_ready == SEM_FAILED)
    {
        /* code */
        perror("sem_open");
        exit(1);
    }

    // loop : wait for messages
    while (1)
    {
        /* code */
        sem_wait(data_ready); //wait for writer
        sem_wait(mutex); //block other process to execute in this critial section.

        printf("Reader read : \nID : %d\nDesc : %s\n\n\n",shared->id,shared->desc);
        sem_post(mutex); //allow other processes to execute in this critial section
        break;
    }

    //clean up 
    munmap(shared,SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);
    sem_unlink(MUTEX_NAME);
    sem_unlink(DATA_READY_NAME);
    
    return 0;
}