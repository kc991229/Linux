#include "tcpsvr.hpp"
#include "pthread.h"
#include <vector>
#include <stdlib.h>

int num=0;
void* threadstart(void* arg)
{
    num++;
    pthread_detach(pthread_self());
    TcpSvr* ts=(TcpSvr*)arg;
    while(1)
    {
        sleep(15);
        std::string buff;
        ts->Recv(&buff);
        printf("client[%d] say:%s\n",num,buff.c_str());
        
        printf("please return client[%d] :\n",num);
        fflush(stdout);
        std::cin>>buff;
        ts->Send(buff);
    }
    ts->Close();
}
int main(int argc,char* argv[])
{
    if (argc!=3)
    {
        printf("Start Client: ./cli [server ip] [server port]\n");
        return 0;
    }

    std::string ip=argv[1];
    uint16_t port=std::atoi(argv[2]);

    TcpSvr ts;
    if (!ts.CreatSocket())
    {
        return 0;
    }
    if (!ts.Bind(ip,port))
    {
        return 0;
    }
    if (!ts.Listen())
    {
        return 0;
    }

    while (1)
    {
        TcpSvr* newts =new TcpSvr();
        struct sockaddr_in addr;
        if (!ts.Accept(&addr,newts))
        {
            continue;
        }
        printf("now have a new connect ,ip=%s,port=%d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));

        pthread_t tid;
        int ret =pthread_create(&tid,NULL,threadstart,(void*)newts);
        if (ret <0)
        {
            return 0;
        }
    }
}
