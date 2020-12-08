#include "tcpsvr.hpp"

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
    
    //1.创建套接字
    if (!ts.CreatSocket())
    {
        return 0;
    }
    if (!ts.Connect(ip,port))
    {
        return 0;
    }
    while (1)
    {
        printf("please enter massger to server:");
        fflush(stdout);
        std::string buff;//用来存放要发送的数据
        std::cin>>buff;
        ts.Send(buff);

        ts.Recv(&buff);
        printf("server say:%s\n",buff.c_str());
    }
    ts.Close();
    return 0;
}
