#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../include/httplib/httplib.h"
#include "../searcher/searcher.hpp"

using std::string;
using std::vector;
using std::unordered_map;
using std::cout;
using std::endl;
int main()
{
    //用httplib库构建一个简单服务器
    using namespace httplib;

    //1.创建Searcher对象
    searcher::Searcher searcher;
    bool ret= searcher.Init("../data/tmp/raw_input");
    if (ret == false)
    {
        cout<<"初始化失败"<<endl;
        return 1;
    }

    //2.创建server对象
    httplib::Server server;
    server.Get("/searcher",[&searcher](const Request& req, Response& resp){
            if(!req.has_param("query"))
            {
                resp.set_content("请求数据错误","text/plain,charset=utf-8");
                return;
            }
            string query=req.get_param_value("query");
            string results;
            searcher.Search("query",&results);
            resp.set_content(results,"application/json;charset=utf-8");
            });
    server.set_base_dir("./wwwroot");
    server.listen("0.0.0.0",1999);
    return 0;
}

