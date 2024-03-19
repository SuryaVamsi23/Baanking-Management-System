
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
};

int main()
{
    int key, semid, i, ret;
    union semun arg;
    static ushort semarray[1] = {1};
    key = ftok(".", 'a');
    semid = semget(key, 1, IPC_CREAT | 0744);
    arg.array = semarray;
    semctl(semid, 0, SETALL, arg);
    for (i = 0; i < 1; i++)
    {
        ret = semctl(semid, i, GETVAL, 0);
        printf("sem %d=%d\n", i, ret);
    }
}