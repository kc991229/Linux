#include "tcpsvr.hpp"
#include <string.h> 
#include <sstream> 
int main(int argc,char* argv[])
{
    if (argc != 3)
    {
        printf("Start Server: ./svr [ip] [port]\n");
    }
    std::string ip=argv[1];
    uint16_t port=atoi(argv[2]);

    TcpSvr us;

    if (!us.CreatSocket())
    {
        return 0;
    }
    if (!us.Bind(ip,port))
    {
        return 0;
    }
    if (!us.Listen(5))
    {
        return 0;
    }

    //为Accept返回值准备
    TcpSvr ser_us;

    while (1)
    {
        struct sockaddr_in addr;
        if (!us.Accept(&addr,&ser_us))
        {
            return 0;
        }
        std::string buff;
        ser_us.Recv(&buff);
        printf("I get %s\n",buff.c_str());
        //正文信息
        std::string body = "<html><h1>test</h1></html>";
        std::stringstream ss;
        ss << "HTTP/1.1 200 OK\r\n";
        ss << "Content-Type: text/html\r\n";
        ss << "Content-Length: " << body.size() << "\r\n";
        ss << "\r\n";
        std::string header = ss.str();

        ser_us.Send(header);
        ser_us.Send(body);
    }
    ser_us.Close();
    us.Close();
    return 0;
}
