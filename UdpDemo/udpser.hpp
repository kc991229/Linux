#pragma once

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Udpser
{
    public:
        Udpser()
        {
            _sock=-1;
        }
        ~Udpser()
        {}
        //1 Socket
        bool Creat_Socket()
        {
            _sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
            if (_sock == -1)
            {
                perror("socket");
                return false;
            }
            return true;
        }
        //2 Bind
        bool Bind(std::string& ip,uint16_t port)
        {
            struct sockaddr_in addr;
            addr.sin_family=AF_INET;
            addr.sin_port=htons(port);//将主机字节序转为网络字节序
            addr.sin_addr.s_addr=inet_addr(ip.c_str());//将字符串转为点分制ip
            int ret=bind(_sock,(struct sockaddr*)&addr,sizeof(addr));
            if (ret < 0)
            {
                perror("bind");
                return false;
            }
            return true;
        }
        //3 Send
        bool Send(std::string& data,sockaddr_in* addr)
        {
            
            int sendsize=sendto(_sock,data.c_str(),data.size(),0,(struct sockaddr*)addr,sizeof(struct sockaddr_in));
            if (sendsize < 0)
            {
                perror("sendto");
                return false;
            }
            return true;
        }
        //4 Recv
        bool Recv(std::string* buf,struct sockaddr_in* addr)
        {
            char tmp[1024];
            memset(tmp,'\0',sizeof(tmp));
            socklen_t socklen=sizeof(struct sockaddr_in);
            int ret=recvfrom(_sock,tmp,sizeof(tmp)-1,0,(struct sockaddr*)addr,&socklen);
            if (ret < 0)
            {
                perror("recvfrom");
                return false;
            }
           (*buf).assign(tmp,ret);
            return true;
        }
        //5 Close
        void Close()
        {
            close(_sock);
            _sock=-1;
        }
    private:
        int _sock;
};
