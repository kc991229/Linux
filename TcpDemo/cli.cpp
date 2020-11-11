#include "tcpsvr.hpp"

int main(int argc,char* argv[])
{
    if (argc!=3)
    {
        printf("Start Client: ./cli [server ip] [server port]\n");
        return 0;
    }
    std::string ip=argv[1];
    uint16_t port=atoi(argv[2]);

    TcpSvr us;
    if (!us.CreatSocket())
    {
        return 0;
    }
    if (!us.Connect(ip,port))
    {
        return 0;
    }
    while (1)
    {
        printf("please enter :");
        fflush(stdout);
        std::string buf;
        std::cin >> buf;
        us.Send(buf);

        us.Recv(&buf);
        printf("I get :%s\n",buf.c_str());
    }
    us.Close();
    return 0;
}
