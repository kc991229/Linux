#include "stdio.h"
#include "unistd.h"
#include "sys/shm.h"

#define shmkey 0x99999999

int main()
{
    int shmid=shmget(shmkey,1024,IPC_CREAT|0664);
    printf("1\n");
    if (shmid < 0)
    {
        perror("shmget");
        return 0;
    }
    struct shmid_ds buff;
    shmctl(shmid,IPC_STAT,&buff);
    printf("shm size =%ld\n",buff.shm_segsz);

    void* adder=shmat(shmid,NULL,0);
    if (!adder)
    {
        perror("shmat");
        return 0;
    }
    while (1)
    {
        fflush(NULL);
        printf("i read [%s]\n",(char*)adder);
        sleep(2);
    }
    shmdt(adder);
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}
