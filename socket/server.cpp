#include "udpser.hpp"


int main(int argc,char* argv[])
{
    if (argc != 3)
    {
         printf("Use this process, please \"./svr [ip] [port]\"");
         return 0;
    }
    std::string ip=argv[1];
    uint16_t port=atoi(argv[2]);
    Udpser us;
    if (us.Creat_Socket() == 0)
        return 0;
    if (us.Bind(ip,port) == 0)
        return 0;

    while (1)
    {
        std::string buff;
        struct sockaddr_in peeraddr;
        us.Recv(&buff,&peeraddr);
        
        printf("chen1 say: %s\n",buff.c_str());
        printf("chen2 say: ");
        fflush(stdout);
        std::cin>>buff;
        us.Send(buff,&peeraddr);
    }
    us.Close();
    return 0;
}
