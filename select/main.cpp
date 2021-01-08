#include "select.hpp"

#define CHECK_RET(p) if(p!=true){return -1;};

int main(int argc,char*argv[])
{
    if (argc!=3)                                                                                                                  
    {
        printf("Start Client: ./cli [server ip] [server port]\n");
        return 0;

    }

    std::string ip=argv[1];
    uint16_t port=std::atoi(argv[2]);

    TcpSvr listen_ts;
    CHECK_RET(listen_ts.CreatSocket());
    CHECK_RET(listen_ts.Bind(ip,port));
    CHECK_RET(listen_ts.Listen());

    SelectSvr ss;
    ss.AddFd(listen_ts.Getfd());

    while (1)
    {
        std::vector<TcpSvr> vec;
        //监控
        if (!ss.SelectWait(&vec))
        {
            continue;
        }

        for (size_t i=0;i<vec.size();i++)
        {
            //1.当接收到新的练级
            if (listen_ts.Getfd()==vec[0].Getfd())
            {
                struct sockaddr_in addr;
                TcpSvr peerts;
                listen_ts.Accept(&addr,&peerts);
                printf("Have a new connect,ip :[%s],port:[%d]",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));

                //将新的套接字放入进行监听
                ss.AddFd(peerts.Getfd());
            }
            //2.当需要处理数据的时候
            else
            {
                std::string data;
                bool ret=vec[i].Recv(&data);
                if (!ret)
                {
                    ss.DeleteFd(vec[i].Getfd());
                    vec[i].Close();
                }
                std::cout<<data<<std::endl;
            }
        }
    }
    return 0;
}
