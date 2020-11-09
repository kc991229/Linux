#include "stdio.h"
#include "unistd.h"
#include "sys/shm.h"
#define shmkey 0x99999999

int main()
{
    int shmid=shmget(shmkey,1024,IPC_CREAT|0664);
    if (shmid < 0)
    {
        perror("shmget");
        return 0;
    }
    void* adder=shmat(shmid,NULL,0);
    if (!adder)
    {
        perror("shmat");
        return 0;
    }
    int i=0;
    while (1)
    {
        fflush(NULL);
        sprintf((char*)adder,"%s -%d","xiao chen",i);
        printf("%s - %d\n","xiao chen",i);
        i++;
        sleep(2);
    }
    shmdt(adder);
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}
