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

blog_system::Table_User* table_user;
blog_system::Table_Tag*  table_tag;
blog_system::Table_Blog* table_blog;

#define WWWROOT  "./www"
void InsertUser(const httplib::Request& req,httplib::Response& rsp)
{
    //用户的信息在req的body中，是一个string,rsp用于返回状态码
    
    //1.获取json字符串
    std::string json_str=req.body;
    //2.将得到的字符串解析成为json_value对象
    Json::Reader reader;
    Json::Value root;
    int ret=reader.parse(json_str,root);
    if (ret == false)
    {
        printf("insert user info parse json faild \n");
        rsp.status=400;//400=bad request 
        return;
    }
    //3.调用table_user接口，将数据插入数据库
    ret =table_user->Insert(root);
    if (ret == false)
    {
        std::cout<<"insert user into db faild"<<std::endl;
        rsp.status=500;
        return;
    }
    //4.填充rsp信息
    rsp.status=200;//200 OK
}
void DeleteUser(const httplib::Request& req,httplib::Response& rsp)
{
    //1.获取用户id，是存放在req.match里的
    int user_id=std::stoi(req.matches[1]);//matched[0]存放的是整个字符串
    //2.执行数据库操作
    bool ret = table_user->Delete(user_id);
    if (ret == false)
    {
        std::cout<<"delete user faild"<<std::endl;
        rsp.status=500;
    }
    rsp.status=200;
    return ;
}
void UpdateUser(const httplib::Request& req,httplib::Response& rsp)
{
    //1.获取用户id,用户操作语句
    int user_id=std::stoi(req.matches[1]);
    std::string str=req.body;
    Json::Reader reader;
    Json::Value value;
    bool ret = reader.parse(str,value);//将字符串放到Json::Value对象中
    if (ret == false)
    {
        std::cout<<"update user error,Json format error"<<std::endl;
        rsp.status=400;
        return;
    }
    //2.执行数据库操作
    ret =table_user->Update(user_id,value);
    if (ret == false)
    {
        std::cout<<"update user error,operator error"<<std::endl;
        rsp.status=500;
        return;
    }
    rsp.status=200;
    return ;
}
void GetAllUser(const httplib::Request& req,httplib::Response& rsp)
{
    //1.从数据库得到所有的用户信息存于Json::Value
    Json::Value root;
    bool ret =table_user->GetAll(&root);
    if (ret == false)
    {
        std::cout<<"get all user info ,db operator error"<<std::endl;
        rsp.status=500;
        return;
    }
    //2.将Json::Value转化为Json字符串，作为rsp信息，即响应
    Json::FastWriter Write;
    std::string str;
    str=Write.write(root);
    //rsp.body=str 将要返回的内容写入
    rsp.set_content(&str[0],str.size(),"application/json");
    return ;
}
void GetOneUser(const httplib::Request& req,httplib::Response& rsp)
{
    int user_id=std::stoi(req.matches[1]);
    Json::Value root;
    bool ret =table_user->GetOne(user_id,&root);
    if (ret ==false)
    {
        printf("get one user info,db operator error\n");
        rsp.status=500;
        return;
    }
    Json::FastWriter Write;
    rsp.set_content(Write.write(root),"application/json");
    return ;
}


void InsertTag(const httplib::Request& req,httplib::Response& rsp)
{
    //用户的信息在req的body中，是一个string,rsp用于返回状态码
    
    //1.获取json字符串
    std::string json_str=req.body;
    //2.将得到的字符串解析成为json_value对象
    Json::Reader reader;
    Json::Value root;
    int ret=reader.parse(json_str,root);
    if (ret == false)
    {
        printf("insert tag info parse json faild \n");
        rsp.status=400;//400=bad request 
        return;
    }
    //3.调用tag_user接口，将数据插入数据库
    ret =table_tag->Insert(root);
    if (ret == false)
    {
        std::cout<<"insert tag into db faild"<<std::endl;
        rsp.status=500;
        return;
    }
    //4.填充rsp信息
    rsp.status=200;//200 OK
    return ;
}
void DeleteTag(const httplib::Request& req,httplib::Response& rsp)
{
    //1.获取用户id，是存放在req.match里的
    int tag_id=std::stoi(req.matches[1]);//matched[0]存放的是整个字符串
    //2.执行数据库操作
    bool ret = table_tag->Delete(tag_id);
    if (ret == false)
    {
        std::cout<<"delete tag faild"<<std::endl;
        rsp.status=500;
    }
    rsp.status=200;
    return ;
}
void UpdateTag(const httplib::Request& req,httplib::Response& rsp)
{
    //1.获取用户id,用户操作语句
    int tag_id=std::stoi(req.matches[1]);
    std::string str=req.body;
    Json::Reader reader;
    Json::Value value;
    bool ret = reader.parse(str,value);//将字符串放到Json::Value对象中
    if (ret == false)
    {
        std::cout<<"update tag error,Json format error"<<std::endl;
        rsp.status=400;
        return;
    }
    //2.执行数据库操作
    ret =table_tag->Update(tag_id,value);
    if (ret == false)
    {
        std::cout<<"update tag error,operator error"<<std::endl;
        rsp.status=500;
        return;
    }
    rsp.status=200;
    return ;
}
void GetAllTag(const httplib::Request& req,httplib::Response& rsp)
{
    //1.从数据库得到所有的用户信息存于Json::Value
    Json::Value root;
    bool ret =table_tag->GetAll(&root);
    if (ret == false)
    {
        std::cout<<"get all tag info ,db operator error"<<std::endl;
        rsp.status=500;
        return;
    }
    //2.将Json::Value转化为Json字符串，作为rsp信息，即响应
    Json::FastWriter Write;
    std::string str;
    str=Write.write(root);
    //rsp.body=str 将要返回的内容写入
    rsp.set_content(&str[0],str.size(),"application/json");
    return ;
}
void GetOneTag(const httplib::Request& req,httplib::Response& rsp)
{
    int tag_id=std::stoi(req.matches[1]);
    Json::Value root;
    bool ret =table_tag->GetOne(tag_id,&root);
    if (ret ==false)
    {
        printf("get one tag info,db operator error\n");
        rsp.status=500;
        return;
    }
    Json::FastWriter Write;
    rsp.set_content(Write.write(root),"application/json");
    return ;
}


void InsertBlog(const httplib::Request& req,httplib::Response& rsp)
{
    //用户的信息在req的body中，是一个string,rsp用于返回状态码
    
    //1.获取json字符串
    std::string json_str=req.body;
    //2.将得到的字符串解析成为json_value对象
    Json::Reader reader;
    Json::Value root;
    int ret=reader.parse(json_str,root);
    if (ret == false)
    {
        printf("insert tag info parse json faild \n");
        rsp.status=400;//400=bad request 
        return;
    }
    //3.调用blog_user接口，将数据插入数据库
    ret =table_blog->Insert(root);
    if (ret == false)
    {
        std::cout<<"insert blog into db faild"<<std::endl;
        rsp.status=500;
        return;
    }
    //4.填充rsp信息
    rsp.status=200;//200 OK
    return ;
}
void DeleteBlog(const httplib::Request& req,httplib::Response& rsp)
{
    //1.获取用户id，是存放在req.match里的
    int blog_id=std::stoi(req.matches[1]);//matched[0]存放的是整个字符串
    //2.执行数据库操作
    bool ret = table_blog->Delete(blog_id);
    if (ret == false)
    {
        std::cout<<"delete blog faild"<<std::endl;
        rsp.status=500;
    }
    rsp.status=200;
    return ;
}
void UpdateBlog(const httplib::Request& req,httplib::Response& rsp)
{
    //1.获取用户id,用户操作语句
    int blog_id=std::stoi(req.matches[1]);
    std::string str=req.body;
    Json::Reader reader;
    Json::Value value;
    bool ret = reader.parse(str,value);//将字符串放到Json::Value对象中
    if (ret == false)
    {
        std::cout<<"update blog error,Json format error"<<std::endl;
        rsp.status=400;
        return;
    }
    //2.执行数据库操作
    ret =table_blog->Update(blog_id,value);
    if (ret == false)
    {
        std::cout<<"update blog error,operator error"<<std::endl;
        rsp.status=500;
        return;
    }
    rsp.status=200;
    return ;
}
void GetAllBlog(const httplib::Request& req,httplib::Response& rsp)
{
    //三种情况，blog、tag_id、user_id
    Json::Value root;
    if (req.has_param("tag_id"))
    {
         int tag_id=std::stoi(req.get_param_value("tag_id"));//得到想要查询的id
        bool ret =table_blog->GetTag(tag_id,&root);//根据得到的id从数据库中得到Json::Value 
        if (ret ==false)
        {
            printf("get tag blog info from db error\n");
            rsp.status=500;
            return;
        }
    }
    else if (req.has_param("user_id"))
    {
        int user_id=std::stoi(req.get_param_value("user_id"));//得到想要查询的id
        bool ret=table_blog->GetUser(user_id,&root);
        if (ret== false)
        {
            printf("get user blog info from db error\n");
            rsp.status=500;
            return;
        }
    }
    else
    {
        bool ret =table_blog->GetAll(&root);
        if (ret == false)
        {
            printf("get all blog info from db error\n");
            rsp.status=500;
            return;
        }
    }
    //将获取的内容放入正文
    Json::FastWriter Write;
    rsp.set_content(Write.write(root),"application/json");
    return ;
}
void GetOneBlog(const httplib::Request& req,httplib::Response& rsp)
{ 
    //1.从req中得到想要获取的博客id
    int blog_id=std::stoi(req.matches[1]);
    //2.从数据库获取内容
    Json::Value root;
    bool ret =table_blog->GetBlog(blog_id,&root);
    if (ret ==false)
    {
        printf("get one blog info from db error\n");
        rsp.status=500;
        return;
    }
    //3.将获得的内容写入返回的正文
    Json::FastWriter Write;
    rsp.set_content(Write.write(root),"application/json");
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
    server.Post(R"(/user)",InsertUser);
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
