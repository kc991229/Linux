#include "udpser.hpp"

int main(int argc,char* argv[])
{
    if (argc != 3)
    {
        printf("./cli [svr_ip] [svr_port]\n");
        return 0;
    }
    std::string ip=argv[1];
    uint16_t port=atoi(argv[2]);
    Udpser us;
    if (us.Creat_Socket() == 0)
        return 0;
    //if (us.Bind(ip,port) == 0)
     //   return 0;
    //组织通信信息
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr =inet_addr(ip.c_str());

    while (1)
    {
        printf("chen1 say: ");
        fflush(stdout);
        std::string buff;
        std::cin >> buff;
        us.Send(buff,&addr);
        struct sockaddr_in peeraddr;
        us.Recv(&buff,&peeraddr);
        printf("chen2 say:%s\n",buff.c_str());
    }
    us.Close();
    return 0;
}
