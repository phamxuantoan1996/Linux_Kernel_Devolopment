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

//     // Create message queue
//     msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
//     if (msgid == -1) {
//         perror("msgget");
//         exit(1);
//     }

//     message.msg_type = 1; // Type = 1
//     strcpy(message.msg_text, "Hello from sender!");

//     // Send message
//     if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
//         perror("msgsnd");
//         exit(1);
//     }

//     printf("Message sent: %s\n", message.msg_text);
//     return 0;
// }


//send struct
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
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
//     message.msg_type = 1;
//     message.data.id = 101;
//     message.data.grade = 8.7;
//     strcpy(message.data.name, "Alice");

//     if (msgsnd(msgid, &message, sizeof(message.data), 0) == -1) {
//         perror("msgsnd");
//         exit(1);
//     }

//     printf("Sent: ID=%d, Grade=%.2f, Name=%s\n",
//            message.data.id, message.data.grade, message.data.name);
//     return 0;
// }


/*-----------------------POSIX Message Queues---------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>   // For O_* constants
#include <sys/stat.h>

struct student {
    int id;
    float grade;
    char name[50];
};

int main() {
    mqd_t mq;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;              // allow up to 10 messages
    attr.mq_msgsize = sizeof(struct student); // each message is one struct
    attr.mq_curmsgs = 0;

    struct student s1 = {101, 9.5, "Alice"};

    // Open or create queue
    mq = mq_open("/myqueue", O_WRONLY | O_CREAT, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Send struct as raw bytes
    if (mq_send(mq, (const char*)&s1, sizeof(s1), 0) == -1) {
        perror("mq_send");
        exit(1);
    }

    printf("Sent: ID=%d, Grade=%.2f, Name=%s\n", s1.id, s1.grade, s1.name);

    mq_getattr(mq,&attr);
    printf("Total number of message is %ld\n",attr.mq_curmsgs);

    mq_close(mq);
    return 0;
}

