// create thread within join
// #include <stdio.h>
// #include <unistd.h>

// #include <pthread.h>

// #include <string.h>
// #include <errno.h>

// void* thread1_func(void* args)
// {
//     printf("thread 1 is runned\n");
//     sleep(1);
//     return NULL;
// }
// int main(int argc,char *argv[])
// {
    
//     pthread_t thread1;
//     printf("main thread is runned\n");
//     if(pthread_create(&thread1,NULL,thread1_func,NULL) != 0)
//     {
//         perror("Failed to create thread");
//         return 1;
//     }
//     pthread_join(thread1,NULL);
//     return 0;
// }


// create thread within detach. Use shared data and mutex to get return value.
// #include <stdio.h>
// #include <unistd.h>
// #include <stdbool.h>
// #include <stdint.h>

// #include <pthread.h>

// #include <string.h>
// #include <error.h>

// typedef struct
// {
//     /* data */
//     bool isDataReady;
//     pthread_mutex_t lock;
//     uint16_t value;
//     uint8_t statusThread;

// } SharedData;

// void* thread1_func(void *arg)
// {
//     SharedData *data = (SharedData*)arg;

//     data->isDataReady = true;
//     for (uint8_t i = 0; i < 10; i++)
//     {
//         pthread_mutex_lock(&data->lock);
//         data->value = i;
//         data->isDataReady = true;
//         pthread_mutex_unlock(&data->lock);
//         /* code */
//         sleep(2);
//     }
//     data->statusThread = 0;
//     printf("Thread 1 is finished.\n");
//     return NULL;
// }
// int main(int argc,char *argv[])
// {
//     SharedData dataThread1;
//     pthread_mutex_t lockDataThread1 =  PTHREAD_MUTEX_INITIALIZER;
//     pthread_t thread1;

//     dataThread1.isDataReady = false;
//     dataThread1.statusThread = 1;
//     dataThread1.lock = lockDataThread1;

//     if(pthread_create(&thread1,NULL,thread1_func,(void*)&dataThread1) != 0)
//     {
//         return 1;
//     }
//     pthread_detach(thread1);

//     while (1)
//     {
//         /* code */
//         pthread_mutex_lock(&dataThread1.lock);
//         if(dataThread1.isDataReady)
//         {
//             printf("value of data thread : %d.\n",dataThread1.value);
//             dataThread1.isDataReady = false;
//         }
//         if (dataThread1.statusThread == 0)
//         {
//             /* code */
//             break;
//         }
//         pthread_mutex_unlock(&dataThread1.lock);
//         sleep(1);
//     }
//     printf("Main thread is finished.\n");

//     return 0;
// }


//Example of Condition Variable
// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <unistd.h>
// #include <stdint.h>
// #include <stdbool.h>

// #define BUFFER_SIZE 5

// uint8_t buffer[BUFFER_SIZE];
// uint32_t count = 0; /*how many item in buffer*/

// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
// pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

// /*Producer thread*/
// void* producer(void* arg)
// {
//     for (uint8_t i = 0; i <= 10; i++)
//     {
//         /* code */
//         pthread_mutex_lock(&lock);
//         /*wait if buffer full*/
//         while (count == BUFFER_SIZE)
//         {
//             /* code */
//             pthread_cond_wait(&not_full,&lock);
//         }
//         buffer[count] = i;
//         count = count + 1;
//         printf("Produced %d (count = %d)\n",i,count);
//         /*wake up one waiting consumer*/
//         pthread_cond_signal(&not_empty);

//         pthread_mutex_unlock(&lock);
//         sleep(2);
//     }
    
//     return NULL;
// }
// /*Consumer thread*/
// void* consumer(void* arg)
// {
//     for (uint8_t i = 0; i <= 10; i++)
//     {
//         /* code */
//         pthread_mutex_lock(&lock);

//         /*wait if buffer empty*/
//         while(count == 0)
//         {
//             pthread_cond_wait(&not_empty,&lock);
//         }

//         count = count - 1;
//         uint8_t item = buffer[count];

//         printf("Consumed %d (count=%d)\n", item, count);

//         /*wake up one waiting producer*/
//         pthread_cond_signal(&not_full);

//         pthread_mutex_unlock(&lock);
//         sleep(3);
//     }
//     return NULL;
// }

// int main(int argc,char *argv[])
// {
//     pthread_t prod, cons;

//     pthread_create(&prod, NULL, producer, NULL);
//     pthread_create(&cons, NULL, consumer, NULL);

//     pthread_join(prod, NULL);
//     pthread_join(cons, NULL);

//     return 0;
// }



// Example of messaage queue between threads.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#include <string.h>
#include <error.h>

#define QUEUE_SIZE 5
typedef struct
{
    uint8_t buffer[QUEUE_SIZE];
    uint32_t front,rear,count;
    pthread_mutex_t lock;
    pthread_cond_t not_full; /*the data type for a pthreads condition variable*/
    pthread_cond_t not_empty;
} MessageQueue;

void mq_init(MessageQueue *q)
{
    q->front = q->rear = q->count = 0;
    pthread_mutex_init(&q->lock,NULL);
    pthread_cond_init(&q->not_empty,NULL);
    pthread_cond_init(&q->not_empty,NULL);
}

void mq_send(MessageQueue *q,int msg)
{
    pthread_mutex_lock(&q->lock);
    /*wait if buffer full*/
    while (q->count == QUEUE_SIZE)
    {
        /* code */
        pthread_cond_wait(&q->not_full,&q->lock);
    }
    q->buffer[q->rear] = msg;
    q->rear = q->rear + 1;
    if(q->rear == QUEUE_SIZE)
    {
        q->rear = 0;
    }
    q->count = q->count + 1;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);
}

int mq_recv(MessageQueue *q)
{
    pthread_mutex_lock(&q->lock);
    while (q->count == 0)
    {
        /* code */
        pthread_cond_wait(&q->not_empty,&q->lock);
    }
    uint8_t msg = q->buffer[q->front];
    q->front = q->front + 1;
    if(q->front == QUEUE_SIZE)
    {
        q->front = 0;
    }
    q->count = q->count - 1;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);
    return msg;
}

void* producer_func(void* arg)
{
    MessageQueue *q = (MessageQueue*)arg;

    for (uint8_t i = 0; i < 255; i++)
    {
        /* code */
        mq_send(q,i);
        printf("Reduced %d\n",i);
        // sleep(1);
        usleep(1000);
    }
    return NULL;
}

void* consumer_func(void* arg)
{
    MessageQueue *q = (MessageQueue*)arg;
    for (uint8_t i = 0; i < 255; i++)
    {
        /* code */
        int msg = mq_recv(q);
        printf("Consumed : %d\n",msg);
        // sleep(1);
        usleep(1050);
    }
    
    return NULL;
}

int main(int argv,char *argc[])
{
    MessageQueue q;
    pthread_t producer,consumer;
    mq_init(&q);

    pthread_create(&producer,NULL,producer_func,(void*)&q);
    pthread_create(&consumer,NULL,consumer_func,(void*)&q);

    pthread_join(producer,NULL);
    pthread_join(consumer,NULL);
    return 0;
}






