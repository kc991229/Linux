#include "tcpsvr.hpp"
#include "epoll_lt.hpp"
#define CHECK_RET(p) if(p!=true){return -1;}

int main()
{
    TcpSvr listen_ts;
    CHECK_RET(listen_ts.CreatSocket());
    CHECK_RET(listen_ts.Bind("0.0.0.0",1999));
    CHECK_RET(listen_ts.Listen());

    epollsvr es;
    CHECK_RET(es.InitSvr());
    es.AddFd(listen_ts.GetFd());

    while (1)
    {
        vector<TcpSvr> vec;
        if (!es.EpollWait(&vec))
        {
            continue;
        }

        for (int i=0;i<vec.size();i++)
        {
            if (vec[i].GetFd() == listen_ts.GetFd())
            {
                struct sockaddr_in addr;
                TcpSvr service;
                listen_ts.Accept(&addr,&service);
                printf("have a new connect,id:[%s],port:[%d]\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));

                //放入新连接
                es.AddFd(service.GetFd());
            }
            else
            {
                std::string buff;
                bool ret =vec[i].Recv(&buff);
                if (!ret)
                {
                    es.DeleteFd(vec[i].GetFd());
                    vec[i].Close();
                }
                printf("get : %s\n",buff.c_str());
            }
        }
    }
        return 0;
}
