#pragma once

#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

class TcpSvr
{
    public:
        TcpSvr()
        {
            _sockfd=-1;
        }
        ~TcpSvr()
        {}
        //1.创建套接字
        bool CreatSocket()
        {
            _sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            if (_sockfd < 0)
            {
                perror("socket");
                return false;
            }
            return true;
        }
        //2.绑定地址信息
        bool Bind(const std::string& ip,uint16_t port)
        {
            struct sockaddr_in addr;
            addr.sin_family=AF_INET;
            addr.sin_port=htons(port);
            addr.sin_addr.s_addr=inet_addr(ip.c_str());
            int ret=bind(_sockfd,(struct sockaddr*)&addr,sizeof(addr));
            if (ret < 0)
            {
                perror("bind");
                return false;
            }
            return true;
        }
        // 3.侦听
        bool Listen(int n=5)
        {
            int ret=listen(_sockfd,n);
            if (ret < 0)
            {
                perror("listen");
                return false;
            }
            return true;
        }
        //4.获取连接
        bool Accept(struct sockaddr_in* addr,TcpSvr* ts)
        {
            socklen_t socklen=sizeof(struct sockaddr_in);
            int serverfd=accept(_sockfd,(struct sockaddr*)addr,&socklen);
            if (serverfd < 0)
            {
                perror("accept");
                return false;
            }
            ts->_sockfd=serverfd;
            return true;
        }
        //5.发起连接
        bool Connect(std::string& ip,uint16_t port)
        {
            struct sockaddr_in addr;
            addr.sin_family=AF_INET;
            addr.sin_port=htons(port);
            addr.sin_addr.s_addr=inet_addr(ip.c_str());
            int ret =connect(_sockfd,(struct sockaddr*)&addr,sizeof(addr));
            if (ret < 0)
            {
                perror("connect");
                return false;
            }
            return true;
        }
        //6.发送数据
        bool Send(std::string& data)
        {
            int sendszie=send(_sockfd,data.c_str(),data.size(),0);
            if (sendszie < 0)
            {
                perror("send");
                return false;
            }
            return true;
        }
        //7.接收数据
        bool Recv(std::string* buff)
        {
            char buf[1024];
            memset(buf,'\0',sizeof(buf));
            int recvsize=recv(_sockfd,buf,sizeof(buf)-1,0);
            if (recvsize < 0)
            {
                perror("recv");
                return false;
            }
            else if (recvsize == 0)//断开连接
            {
                printf("The connection has been disconnected\n");
                return false;
            }
            else
            {
                (*buff).assign(buf,recvsize);
                return true;
            }
        }
            //8.关闭
            void Close()
            {
                close(_sockfd);
                _sockfd=-1;
            }
            void SetFd(int fd)
            {
                _sockfd=fd;
            }
            int GetFd()
            {
                return _sockfd;
            }
    private:
        int _sockfd;
};
