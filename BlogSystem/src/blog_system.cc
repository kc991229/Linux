#include "db.hpp"
#include "httplib.h"
void test_mysql()
{
     MYSQL* mysql= blog_system::MysqlInit();
    blog_system::Table_User table_user(mysql);
    Json::Value user;
    user["name"]="康琛男神";
    table_user.Update(1,user);
  //  table_user.GetAll(&user);
    Json::Reader reader;
    Json::StyledWriter writer;
    std::cout<<writer.write(user)<<std::endl;
   // user["name"]="康琛";
   // table_user.Insert(user);
   // user["name"]="刘建波";
   // table_user.Insert(user);
}
void test_http()
{
    using namespace httplib;
    Server server;
    server.Get("/",[](const Request& req, Response& res) {
                res.set_content("<h1>kangchen<h1>", "text/html");
              });
    server.listen("0.0.0.0",1999);
}
void test_http1()
{
    httplib::Server svr;

    svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
              res.set_content("Hello World!", "text/plain");

            });

    svr.listen("0.0.0.0", 8080);
}

blog_system::Table_User* table_user;
blog_system::Table_Tag*  table_tag;
blog_system::Table_Blog* table_blog;

#define WWWROOT  "./www"
void InsertUser(httplib::Request& req,httplib::Response& rsp)
{
    std::cout<<req.method<<std::endl;
    std::cout<<req.body<<std::endl;
    std::cout<<req.path<<std::endl;

    for (auto it=req.headers.begin();it!=req.headers.end();it++)
    {
        std::cout<<it->first<<"="<<it->second<<std::endl;
    }
    for (auto it=req.params.begin();it!=req.headers.end();it++)
    {
        std::cout<<it->first<<"="<<it->second<<std::endl;
    }
    return ;
}
void DeleteUser(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void UpdateUser(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void GetAllUser(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void GetOneUser(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}


void InsertTag(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void DeleteTag(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void UpdateTag(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void GetAllTag(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void GetOneTag(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}


void InsertBlog(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void DeleteBlog(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void UpdateBlog(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void GetAllBlog(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}
void GetOneBlog(const httplib::Request& req,httplib::Response& rsp)
{
    return ;
}


int main()
{
   MYSQL* mysql = blog_system::MysqlInit();
    if (mysql == NULL)
    {
        return -1;
    }
    table_blog= new blog_system::Table_Blog(mysql);
    table_tag = new blog_system::Table_Tag(mysql);
    table_user= new blog_system::Table_User(mysql);

    httplib::Server server;
    server.set_base_dir(WWWROOT);//设置url中的资源路径的相对根目录
    //好处是一些根目录下的静态文件资源。httplib会自动进行读取响应

    //采用正则表达式
    server.Post(R"(/user)",InsertTag);
    server.Delete(R"(/user/(\d+))",DeleteUser);
    server.Put(R"(/user/(\d+))",UpdateUser);
    server.Get(R"(/user)",GetAllUser);
    server.Get(R"( /user/(\d+))",GetOneUser);

    server.Post(R"(/tag)",InsertTag);
    server.Delete(R"(/tag/(\d+))",DeleteTag);
    server.Put(R"(/tag/(\d+))",UpdateTag);
    server.Get(R"(/tag)",GetAllTag);
    server.Get(R"(/tag/(\d+))",GetOneTag);

    
    server.Post(R"(/blog)",InsertBlog);
    server.Delete(R"(/blog/(\d+))",DeleteBlog);
    server.Put(R"(/blog/(\d+))",UpdateBlog);
    server.Get (R"(/blog)",GetAllBlog);
    server.Get(R"(/blog/(\d+))",GetOneBlog);


    server.listen("0.0.0.0",1999);

    blog_system::MysqlRelease(mysql);
    return 0;
}
