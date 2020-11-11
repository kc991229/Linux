#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>


#define THREADCOUNT 4

typedef void* (*Handler_t)(int);//一个函数指针的宏
class ThreadTask
{
    public:
        ThreadTask(int data, Handler_t handler)
        {
            data_ = data;
            handler_ = handler;                                            
        }
        ~ThreadTask()
        {                                
        }
        //从这个接口开始运行
        void Run()
        {
            handler_(data_);                                                
        }
    private:
        int data_;
        Handler_t handler_; //返回值为void* 参数为int
};

class ThreadPool
{
    public:
        ThreadPool()
        {
            capacity_ = 10;
            thread_capacity_ = THREADCOUNT;
            pthread_mutex_init(&lock_, NULL);
            pthread_cond_init(&cond_, NULL);

            bool is_create = ThreadCreate();
            if(!is_create)
            {
                printf("Threadpool Create thread failed\n");
               // exit(1);
               
            }

            IsExit = false;

            cur_thread_count_ = THREADCOUNT;                                                                                                                    
        }

        ~ThreadPool()
        {                           
        }
        //入线程池
        bool Push(ThreadTask* tt)
        {
            pthread_mutex_lock(&lock_);
            if(IsExit)
            {
                pthread_mutex_unlock(&lock_);
                return false;

            }
            que_.push(tt);
            pthread_mutex_unlock(&lock_);

            pthread_cond_signal(&cond_);
            return true;                                                                                                            
        }
        //出线程池
        bool Pop(ThreadTask** tt)
        {
            *tt = que_.front();
            que_.pop();
            return true;                                                                                
        }
        void ThreadJoin()
        {
            for(int i = 0; i < THREADCOUNT; i++)
            {
                pthread_join(tid_[i], NULL);                                                                                    
            }

        }

        void ThreadExit()
        {
            for(int i = 0; i < THREADCOUNT; i++)
            {
                pthread_cancel(tid_[i]);                                                                                            
            }
                                                                                    
        }

        void ThreadPoolClear()
        {

            pthread_mutex_lock(&lock_);
            IsExit = true;
            pthread_mutex_unlock(&lock_);
                                        
            if(cur_thread_count_ > 0)
            {
                pthread_cond_broadcast(&cond_);
            }
        }                                        
    private: 
        static void* ThreadStart(void* arg)    
        {                               
            ThreadPool* tp = (ThreadPool*)arg;
            while(1)                                  
            {
                pthread_mutex_lock(&tp->lock_);
                ThreadTask* tt;
                while(tp->que_.empty())
                {
                    if(tp->IsExit)
                    {
                        tp->cur_thread_count_--;
                        pthread_mutex_unlock(&tp->lock_);                        
                        pthread_exit(NULL);                                                                               
                    }                                                   
                    pthread_cond_wait(&tp->cond_, &tp->lock_);                                                                                                                                                                                                   
                }
                                                                                                                            
                tp->Pop(&tt);                                                                                                                                                                            
                pthread_mutex_unlock(&tp->lock_);
                tt->Run();
                delete tt;
            }
                                                                                                                
        }

        bool ThreadCreate()
        {
            for(int i = 0; i < THREADCOUNT; i++)
            {
                int ret = pthread_create(&tid_[i], NULL, ThreadStart, (void*)this);
                if(ret != 0)
                {                                                             
                    perror("pthread_create");      
                    return false;                                                                                               
                }                                                                                                                
            }
                                                                                                            
            return true;                                                                                                            
    }
    private:
        std::queue<ThreadTask*> que_;
        size_t capacity_;
        pthread_mutex_t lock_;
        pthread_cond_t cond_;
        size_t thread_capacity_;
        size_t cur_thread_count_;
        pthread_t tid_[THREADCOUNT];
        bool IsExit;
};

void* DealData(int data)
{
    printf("consume data is %d\n", data);
    return NULL;
}

int main()
{
     ThreadPool* tp = new ThreadPool();
     for(int i = 1; i <= 50; i++)
     {  
         ThreadTask* tt = new ThreadTask(i, DealData);
         tp->Push(tt);                           
     }
     sleep(15);
     tp->ThreadPoolClear();
     tp->ThreadJoin();
     delete tp;
     return 0;
}
