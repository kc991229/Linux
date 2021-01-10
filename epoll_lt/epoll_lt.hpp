#pragma once 

#include <stdlib.h>
#include <vector>
#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>

#include "tcpsvr.hpp"

using std::vector;

class epollsvr
{
    public:
        epollsvr()
        {
            epoll_fd=-1;
        }
        ~epollsvr()
        {}
        bool InitSvr()
        {
            epoll_fd=epoll_create(10);
            if (epoll_fd < 0)
                return false;
            return true;
        }

        bool AddFd(int fd)
        {
            struct epoll_event ev;
            ev.events=EPOLLIN|EPOLLET;
            ev.data.fd=fd;
            int ret=epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&ev);
            if (ret<0)
            {
                perror("epoll_ctl");
                return false;
            }
            return true;
        }

        bool DeleteFd(int fd)
        {
            int ret=epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,NULL);
            if (ret < 0)
            {
                perror("epool_ctl");
                return false;
            }
            return true;
        }

        //输出型参数
        bool EpollWait(vector<TcpSvr>* out)
        {
            struct epoll_event fd_array[10];
            unsigned long ret=epoll_wait(epoll_fd,fd_array,sizeof(fd_array)/sizeof(fd_array[0]),-1);
            if (ret < 0)
            {
                perror("epoll_wait");
                return false;
            }
            else if (ret == 0)
            {
                printf("time out\n");
                return false;
            }

            //防止越界
            if(ret > sizeof(fd_array)/sizeof(fd_array[0]))
            {
                ret = sizeof(fd_array)/sizeof(fd_array[0]);                            
            }

            for (int i=0;i<ret;i++)
            {
                TcpSvr ts;
                ts.SetFd(fd_array[i].data.fd);
                out->push_back(ts);
            }
            return true;
        }
    private:
        int epoll_fd;
};
