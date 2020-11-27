#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#include <cstdio>

#define THREADCOUNT 2
#define TypeData int
//这里使用泛型编程
template <class T>
class BlockQueue
{
    public:
        BlockQueue(int capacity)
        {
            _capacity=capacity;
            pthread_mutex_init(&_lock,NULL);
            pthread_cond_init(&_consumer_cond,NULL);
            pthread_cond_init(&_producer_cond,NULL);
        }
        ~BlockQueue()
        {
            pthread_mutex_destroy(&_lock);
            pthread_cond_destroy(&_consumer_cond);
            pthread_cond_destroy(&_producer_cond);
        }
        //入队列
        void Push(T& data)
        {
            pthread_mutex_lock(&_lock);
            while (IsFull())
            {
                pthread_cond_wait(&_producer_cond,&_lock);
            }
            _aq.push(data);
            pthread_mutex_unlock(&_lock);
            pthread_cond_signal(&_consumer_cond);
        }
        //出队列
        void Pop(T* data)
        {
            pthread_mutex_lock(&_lock);
            while (_aq.empty())
            {
                pthread_cond_wait(&_consumer_cond,&_lock);
            }
            *data=_aq.front();
            _aq.pop();
            pthread_mutex_unlock(&_lock);
            pthread_cond_signal(&_producer_cond);
        }
    private:
        //判断队列是否满了
        bool IsFull()
        {
            if (_aq.size() == _capacity)
                return true;
            else
                return false;
        }
    private:
        size_t _capacity;//容量
        std::queue<T> _aq;//容量
        pthread_mutex_t _lock;//互斥
        pthread_cond_t _consumer_cond;//同步
        pthread_cond_t _producer_cond;//同步
};
//消费者线程
void* consumerstart(void* arg)
{
    BlockQueue<TypeData>* bq=(BlockQueue<TypeData>*)arg;
    while (1)
    {
        TypeData data;
        bq->Pop(&data);
        std::cout<<"I am consumer["<<pthread_self()<<"], I get "<<data<<std::endl;
    }
    return NULL;
}
//生产者线程
void* producerstart(void* arg)
{
    BlockQueue<TypeData>* bq=(BlockQueue<TypeData>*)arg;
    TypeData i=0;
    while(1)
    {
       bq->Push(i);
       std::cout<<"I am producer["<<pthread_self()<<"], I product "<<i<<std::endl;
       i++;
    }
    return NULL;
}

int main()
{
    BlockQueue<TypeData>* bq=new BlockQueue<TypeData>(10);
    //用于存放线程tid
    pthread_t consumer_tid[THREADCOUNT];
    pthread_t producer_tid[THREADCOUNT];
    int i=0,ret=0;
    for (;i<THREADCOUNT;i++)
    {
       ret= pthread_create(&consumer_tid[i],NULL,consumerstart,(void*)bq);
       if (ret < 0)
       {
           perror("pthread_create");
           return 0;
       }
    }
    for (i=0;i<THREADCOUNT;i++)
    {
        ret = pthread_create(&producer_tid[i],NULL,producerstart,(void*)bq);
        if (ret < 0)
        {
            perror("pthread_create");
            return 0;
        }
    }
    for (i=0;i<THREADCOUNT;i++)
    {
        pthread_join(producer_tid[i],NULL);
        pthread_join(consumer_tid[i],NULL);
    }
    delete bq;
    bq=NULL;
    return 0;
}
