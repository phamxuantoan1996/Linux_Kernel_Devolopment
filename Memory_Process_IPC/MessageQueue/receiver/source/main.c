/*-----------------------System V Message Queue---------------------------*/

// send text
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/ipc.h>
// #include <sys/msg.h>
// #define MSG_KEY 1234
// struct msg_buffer {
//     long msg_type;
//     char msg_text[100];
// };
// int main() {
//     int msgid;
//     struct msg_buffer message;

//     // Connect to existing queue
//     msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
//     if (msgid == -1) {
//         perror("msgget");
//         exit(1);
//     }

//     // Receive message of type 1
//     if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0) == -1) {
//         perror("msgrcv");
//         exit(1);
//     }

//     printf("Message received: %s\n", message.msg_text);

//     // Cleanup: remove queue
//     msgctl(msgid, IPC_RMID, NULL);

//     return 0;
// }


// send struct
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/ipc.h>
// #include <sys/msg.h>
// #define MSG_KEY 1234
// struct student {
//     int id;
//     float grade;
//     char name[50];
// };
// struct msg_buffer {
//     long msg_type;
//     struct student data;
// };
// int main() {
//     int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
//     if (msgid == -1) { perror("msgget"); exit(1); }

//     struct msg_buffer message;

//     if (msgrcv(msgid, &message, sizeof(message.data), 1, 0) == -1) {
//         perror("msgrcv");
//         exit(1);
//     }

//     printf("Received: ID=%d, Grade=%.2f, Name=%s\n",
//            message.data.id, message.data.grade, message.data.name);

//     // Cleanup
//     msgctl(msgid, IPC_RMID, NULL);
//     return 0;
// }


/*-----------------------POSIX Message Queues---------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

struct student {
    int id;
    float grade;
    char name[50];
};

int main() {
    mqd_t mq;
    struct mq_attr attr;
    struct student s2;

    // Open queue for reading
    mq = mq_open("/myqueue", O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Receive struct
    if (mq_receive(mq, (char*)&s2, sizeof(s2), NULL) == -1) {
        //if message queue is empty.
        perror("mq_receive");
        exit(1);
    }

    // Get information of message queue
    mq_getattr(mq,&attr);
    printf("Received: ID=%d, Grade=%.2f, Name=%s\n", s2.id, s2.grade, s2.name);
    printf("Number of message remaining is %ld\n",attr.mq_curmsgs);
    mq_close(mq);
    // if (attr.mq_curmsgs == 0)
    // {
    //     /* code */
    //     printf("Unlink message queue.\n");
    //     mq_unlink("/myqueue"); // Remove queue

    // }
    return 0;
}
