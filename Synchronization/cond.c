#include "stdio.h"
#include "unistd.h"
#include "pthread.h"

#define count 2
int g_noodles = 0;
pthread_mutex_t lock;
pthread_cond_t producer_cond;
pthread_cond_t consumer_cond;

//消费者
void* Consumer_start()
{
    //上锁、判断、等待、解锁、唤醒
    while (1)
    {
        pthread_mutex_lock(&lock);
        while (g_noodles == 0)
        {
            //等待
            pthread_cond_wait(&consumer_cond,&lock);
        }
        usleep(100000);
        g_noodles--;
        printf("I am consumer,noodles count is  %d\n",g_noodles);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&producer_cond);
    }
    return NULL;
}
//生产者
void* Producer_start()
{
    //上锁、判断、等待、解锁、唤醒
    while (1)
    {
        pthread_mutex_lock(&lock);
        while (g_noodles != 0)
        {
            pthread_cond_wait(&producer_cond,&lock);
        }
        g_noodles++;
        usleep(100000);
        printf("I am producer,noodles count is %d\n",g_noodles);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&consumer_cond);
    }
    return NULL;
}


int main()
{
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&consumer_cond,NULL);
    pthread_cond_init(&producer_cond,NULL);

    pthread_t consumer_tid[count];
    pthread_t producer_tid[count];
    int i=0,ret=-1;
    for (;i<count;i++)
    {
        ret = pthread_create(&consumer_tid[i],NULL,Consumer_start,NULL);
        if (ret < 0)
        {
            perror("pthread_creat");
            return 0;
        }
    }

    for (i=0;i<count;i++)
    {
        ret = pthread_create(&producer_tid[i],NULL,Producer_start,NULL);
        if (ret < 0)
        {
            perror("pthread_creat");
            return 0;
        }
    }

    for (i=0;i<count;i++)
    {
        pthread_join(producer_tid[i],NULL);
        pthread_join(consumer_tid[i],NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&consumer_cond);
    pthread_cond_destroy(&producer_cond);
    return 0;
}
