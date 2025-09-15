# 1. Creation Thread
include pthread.h library

pthread_creatd() function is used to creat a thread.

# The difference between pthread_join() and pthread_detach() function.
pthread_join function:
+ used by one thread (usually main thread) to wait for another the thread to finish.
+ After the target thread terminate, its exit status can be collected.
+ Prevent the terminated thread from becoming a zombie.

phread_detach function:
+ Mark a thread as detached -> it will clean up it own resource when it finish.
+ Useful for background/daemon tash where don't care about the return value.

# Note
if you don't join or detach, then:
+ when a thread finishes  execution, it goes into  a "zombie" state. In this state, stack, register and memory are freed. But some thread metadate (thread id, exit status) is kept by OS. This will causes resouce leak.
+ If you keep creating threads without cleaning up them, you'll eventually hit the maximum number of threads and your program will fail with error like: pthread_create : Resource temporarily unavailable.

Once a thread is detached, you can't pthread_join() it anymore. That means you lose the ability to get its return value and exit status. If you must detach, you need another way to communicate results:
+ Shared memory (global or pass-in struct) protected with pthread_mutex_t.
+ Message queue/pipe
+ Condition variable.

# Condition variable
A condition variable is a synchroniztionvpromitive that lets threads wait for some condition to become true :
+ Mutex protects access to shared state.
+ Condition variable allows threads to wait until another thread signal that something changed.

Functions:
+ pthread_con_init() : initialize condition variale
+ pthread_con_wait() : Atomically unlocks the mutex and waits.
+ When signaled, it locks the mutex again before returning.



# Message queue
Message queue are a classic way for threads of processes to communicate safely without variable directly.

There are two major contexts for message queues in C:
+ Between threads in the same process -> you usually implement your own (protected by mutexes/conditional variables).

+ Between different processes -> use the POSIX message queue API (mqueue.h) or the System V IPC message queue API (sys/msg.h).

+ pthread_cond_signal(pthread_cond_t *cond) → wakes one waiting thread.

+ pthread_cond_broadcast(pthread_cond_t *cond) → wakes all waiting threads.

+ pthread_cond_destroy(pthread_cond_t *cond) → cleanup.

# Pipe