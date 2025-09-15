#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h> //permission of file
#include <wait.h>

#include <string.h>
#include <errno.h>

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    /* data */
    int8_t value1;
    bool value2;
    float value3;
    char desc[100];
} DataBackup;

int readDataBackup(const char *path,DataBackup *backup)
{
    ssize_t num_of_byte;
    int fd = open("file1.bin",O_RDONLY);
    if (fd < 0)
    {
        /* code */
        perror("Error in while open file");
        return 1;
    }
    num_of_byte = read(fd,(void*)backup,sizeof(DataBackup));
    close(fd);
    if (num_of_byte != sizeof(DataBackup))
    {
        /* code */
        return 1;
    }
    return 0;
}

int main(int argc,char *argv[])
{
    printf("read data backup.\n");

    int pid = fork();
    if (pid == 0)
    {
        /* child */
        DataBackup backup;
        if(readDataBackup("file1.bin",&backup) != 0)
        {
            exit(1);
        }
        printf("desc : %s\n",backup.desc);
        exit(0);
        }
    else
    {
        if (pid > 0)
        {
            /* parent */
            int status;
            wait(&status);
        }
        else
        {
            /* error */
            perror("Error in while creating process");
        }
    }
    

    return 0;
}