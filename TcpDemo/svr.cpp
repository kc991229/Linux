#include "tcpsvr.hpp"

int main(int argc,char* argv[])
{
    if (argc != 3)
    {
        printf("Start Server: ./svr [ip] [port]\n");
        return 0;
    }
    std::string ip=argv[1];
    uint16_t port=atoi(argv[2]);

    TcpSvr us;
    if (! us.CreatSocket())
    {
        return 0;
    }
    if (! us.Bind(ip,port))
    {
        return 0;
    }
    if (! us.Listen(5))
    {
        return 0;
    }

    TcpSvr sursock;
    while (1)
    {
        struct sockaddr_in clientaddr;
        if (! us.Accept(&clientaddr,&sursock))
        {
            return 0;
        }

        std::string buff;
        sursock.Recv(&buff);
        printf("I get:%s\n",buff.c_str());

        printf("please enter :");
        fflush(stdout);
        std::cin>>buff;
        sursock.Send(buff);
    }
    us.Close();
    sursock.Close();
    return 0;
}
