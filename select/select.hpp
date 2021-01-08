#pragma once
#include <stdio.h>
#include <sys/select.h>
#include <vector>
#include <unistd.h>

#include "tcpsvr.hpp"

class SelectSvr
{
    public:
        SelectSvr()
        {
            max_fd=-1;
            FD_ZERO(&readfds);
        }
        ~SelectSvr()
        {}
        void AddFd(int fd)
        {
            //添加
            FD_SET(fd,&readfds);
            //更新
            if (fd>max_fd)
            {
                max_fd=fd;
            }
        }
        void DeleteFd(int fd)
        {
            //删除
            FD_CLR(fd,&readfds);
            //更新最大描述符
            if (fd==max_fd)
            {
                for (int i=max_fd;i>=0;i--)
                {
                    if(FD_ISSET(i,&readfds)==0)
                    {
                        continue;
                    }
                    max_fd=i;
                    break;
                }
            }
        }
        bool SelectWait(std::vector<TcpSvr>* vec)
        {
            struct timeval val;
            val.tv_sec=0;
            val.tv_usec=3000;

            fd_set tmp=readfds;//防止返回时将未标记的清除
            int ret=select(max_fd+1,&tmp,NULL,NULL,&val);
            if (ret<0)
            {
                perror("select");
                return false;
            }
            if (ret==0)
            {
                printf("select timeout\n");
                return false;
            }

            for (int i=0;i<=max_fd;i++)
            {
                if (FD_ISSET(i,&tmp))
                {
                    TcpSvr ts;
                    ts.Setfd(i);
                    vec->push_back(ts);
                }
            }
            return true;
        }
    private:
        int max_fd;
        fd_set readfds;
};


