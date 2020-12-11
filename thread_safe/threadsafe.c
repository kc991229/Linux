#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#define count 4

int tickes = 100;
pthread_mutex_t  lock;

void* threadstart(void* arg)
{
    (void)arg;
    while (1)
    {
        pthread_mutex_lock(&lock);
        if (tickes > 0)
        {
            tickes--;
            usleep(100000);
            printf("I am thread [%p],i get tickes [%d]\n",pthread_self(),tickes+1);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            break;
        }
            pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main()
{
    int i=0;
    pthread_mutex_init(&lock,NULL);
    pthread_t tid[count];
    for ( i=0;i<count;i++)
    {
        int ret=pthread_create(&tid[i],NULL,threadstart,NULL);
        if (ret < 0 )
        {
            perror("pthread_create");
            return 0;
        }
    }

        for ( i=0;i<count;i++)
        {
            pthread_join(tid[i],NULL);
        }

        pthread_mutex_destroy(&lock);
    return 0;
}