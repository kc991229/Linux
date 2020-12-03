#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <mysql/mysql.h>
#include <jsoncpp/json/json.h>

#define MYSQL_HOST   "localhost"
#define MYSQL_DB     "blog_system"
#define MYSQL_USER   "root"
#define MYSQL_PASS   "thunder1314"

namespace blog_system
{
    static MYSQL* MysqlInit()
    {
        MYSQL* mysql=NULL;
            //1.初始化句柄 接口函数:mysql_init 
        mysql=mysql_init(NULL);
        if (mysql == NULL)
        {
            printf("mysql error\n");
           return NULL;
        }

            //2.连接mysql服务器 接口函数：mysql_real_connect
         if (mysql_real_connect(mysql,MYSQL_HOST,MYSQL_USER,MYSQL_PASS,MYSQL_DB,0,NULL,0) == NULL )
        {
            printf("connect error,%s\n",mysql_error(mysql));    
            mysql_close(mysql);
            return NULL;
         }
            //3.设置字符集 接口函数：mysql_set_character_set
        if (mysql_set_character_set(mysql,"utf8") != 0)
        {
            printf("set character error %s\n",mysql_error(mysql));
            mysql_close(mysql);
            return NULL;
        }
            
        return mysql;
        }
    //关闭
    static void MysqlRelease(MYSQL* mysql)
    {
        if (mysql)
        {
            mysql_close(mysql);
        }
    }
    //执行(查询)函数，函数接口：mysql_query 
    static bool MysqlQuery(MYSQL* mysql,const std::string& sql)
    {
        int ret =mysql_query(mysql,sql.c_str());
        if (ret != 0)
        {
            printf("query error,%s\n",mysql_error(mysql));
            return false;
        }
        return true;
    }

    class Table_Tag
    {
        public:
            Table_Tag(MYSQL* mysql)
                :_mysql(mysql)
            {}
            //插入函数
            bool Insert(const Json::Value& tag)
            {
#define INSERT_TAG "insert table_tag values(null,'%s');"
                char tmp[4096]={0};
                //tag["name"].asCString()表示以c风格返回
                sprintf(tmp,INSERT_TAG,tag["name"].asCString());
                bool ret = blog_system::MysqlQuery(_mysql,tmp);
                if(ret == false)
                {
                    return false;
                }
                return true;
            }

            bool Delete(int tag_id)
            {
#define DELETE_TAG "delete from table_tag where id =%d;"
                char tmp[4096]={0};
                sprintf(tmp,DELETE_TAG,tag_id);
                bool ret = blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                {
                    return false;
                }
                return true;
            }

            bool Update(int tag_id,Json::Value& tag)
            {
#define UPDATE_TAG "update table_tag set name ='%s' where id ='%d';"
                char tmp[4096]={0};
                sprintf(tmp,UPDATE_TAG,tag["name"].asCString(),tag_id);
                bool ret =blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                    return false;
                return true;
            }

            bool GetAll(Json::Value* tags)
            {
#define SELECT_TAGALL "select * from table_tag;"
                int i=0;
                bool ret = blog_system::MysqlQuery(_mysql,SELECT_TAGALL);
                if (ret == false)
                {
                    return false; 
                }
                MYSQL_RES* res=mysql_store_result(_mysql);//获取结果集
                int num_row=mysql_num_rows(res);//获取结果集的行数
                if (num_row == 0)
                {
                    printf("No information\n");
                    mysql_free_result(res);
                    return false;
                }   
                for (;i<num_row;i++)
                {
                    MYSQL_ROW row = mysql_fetch_row(res);//遍历结果集,获取每一行数据
                    Json::Value tag;
                    tag["id"]=std::stoi(row[0]);
                    tag["name"]=row[i];
                    tags->append(tag);//将Json_value对象插入返回
                }
                mysql_free_result(res);//释放结果集，否则会造成内存泄漏
                return true;
            }
            bool GetOne(int tag_id,Json::Value* tag)
            {
#define SELECT_TAGONE "select name from table_tag where id =%d;"
                char tmp[4096]={0};
                sprintf(tmp,SELECT_TAGONE,tag_id);
                bool ret = blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                    return false;
                MYSQL_RES* res=mysql_store_result(_mysql);//获取结果集
                int num_row=mysql_num_rows(res);//获取结果集条数
                if (num_row != 1)
                {
                    printf("tag count errror\n");
                    mysql_free_result(res);
                    return false;
                }
                MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每行数据
                (*tag)["id"]=tag_id;
                (*tag)["name"]=row[0];
                mysql_free_result(res);
                return true;
            }
        private:
            MYSQL* _mysql;
    };
    class Table_User
    {
        public:
            Table_User(MYSQL* mysql)
                :_mysql(mysql)
            {}

            //插入函数
            bool Insert(const Json::Value& user)
            {
#define INSERT_USER "insert table_user values(null,'%s');"
                char tmp[4096]={0};
                //user["name"].asCString()表示以c风格返回
                sprintf(tmp,INSERT_USER,user["name"].asCString());
                bool ret = blog_system::MysqlQuery(_mysql,tmp);
                if(ret == false)
                {
                    return false;
                }
                return true;
            }

            bool Delete(int user_id)
            {
#define DELETE_USER "delete from table_user where id =%d;"
                char tmp[4096]={0};
                sprintf(tmp,DELETE_USER,user_id);
                bool ret = blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                {
                    return false;
                }
                return true;
            }

            bool Update(int user_id,Json::Value& user)
            {
#define UPDATE_USER "update table_user set name ='%s' where id ='%d';"
                char tmp[4096]={0};
                sprintf(tmp,UPDATE_USER,user["name"].asCString(),user_id);
                bool ret =blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                    return false;
                return true;
            }

            bool GetAll(Json::Value* users)
            {
#define SELECT_USERALL "select * from table_user;"
                int i=0;
                bool ret = blog_system::MysqlQuery(_mysql,SELECT_USERALL);
                if (ret == false)
                {
                    return false; 
                }
                MYSQL_RES* res=mysql_store_result(_mysql);//获取结果集
                int num_row=mysql_num_rows(res);//获取结果集的行数
                if (num_row == 0)
                {
                    printf("No information\n");
                    mysql_free_result(res);
                    return false;
                }   
                for (;i<num_row;i++)
                {
                    MYSQL_ROW row = mysql_fetch_row(res);//遍历结果集,获取每一行数据
                    Json::Value user;
                    user["id"]=std::stoi(row[0]);
                    user["name"]=row[i];
                    users->append(user);//将Json_value对象插入返回
                }
                mysql_free_result(res);//释放结果集，否则会造成内存泄漏
                return true;
            }
            bool GetOne(int user_id,Json::Value* user)
            {
#define SELECT_USERONE "select name from table_user where id ='%d';"
                char tmp[4096]={0};
                sprintf(tmp,SELECT_USERONE,user_id);
                bool ret = blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                    return false;
                MYSQL_RES* res=mysql_store_result(_mysql);//获取结果集
                int num_row=mysql_num_rows(res);//获取结果集条数
                if (num_row != 1)
                {
                    printf("user count errror\n");
                    mysql_free_result(res);
                    return false;
                }
                MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每行数据
                (*user)["id"]=user_id;
                (*user)["name"]=row[0];
                mysql_free_result(res);
                return true;
            }
        private:
            MYSQL* _mysql;
    };
    class Table_Blog
    {
        public:
            Table_Blog(MYSQL* mysql)
                :_mysql(mysql)
            {}

            //插入函数
            bool Insert(const Json::Value& blog)
            {
                //id  tag_id user_id title  content
#define INSERT_BLOG "insert table_blog values(null,%d,%d,'%s','%s',now());"
                char tmp[4096]={0};
                //user["name"].asCString()表示以c风格返回
                sprintf(tmp,INSERT_BLOG,blog["tag_id"].asInt(), blog["user_id"].asInt(),blog["title"].asCString(), blog["content"].asCString());
                std::cout<<tmp<<std::endl;
                bool ret = blog_system::MysqlQuery(_mysql,tmp);
                if(ret == false)
                {
                    return false;
                }
                return true;
            }
            bool Delete(int blog_id)
            {
#define DELETE_BLOG "delete from table_blog where id ='%d';"
                char tmp[4096]={0};
                sprintf(tmp,DELETE_BLOG,blog_id);
                bool ret = blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                {
                    return false;
                }
                return true;
            }
            bool Update(int blog_id,Json::Value& blog)
            {
#define UPDATE_BLOG "update table_blog set tag_id ='%d',user_id='%d',title='%s',content='%s' where id ='%d';"
                char tmp[4096]={0};
                sprintf(tmp,UPDATE_BLOG,blog["tag_id"].asInt(),blog["user_id"].asInt(),blog["title"].asCString(),blog["content"].asCString(),blog_id);
                bool ret =blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                    return false;
                return true;
            }
            bool GetAll(Json::Value* blogs)
            {
                int i=0;
#define SELECT_BLOG_ALL "select * from table_blog;"
                bool ret =blog_system::MysqlQuery(_mysql,SELECT_BLOG_ALL);
                if (ret == false)
                {
                    return false;
                }
                MYSQL_RES* res=mysql_store_result(_mysql);//获取结果集
                int num_row=mysql_num_rows(res);//获取行数
                if (num_row <= 0)
                {
                    printf("blog result info error\n");
                    mysql_free_result(res);
                    return false;
                }
                
                //id tag_id user_id title content ctime
                for (;i< num_row;i++)
                {
                    MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
                    Json::Value blog;
                    blog["id"]=std::stoi(row[0]);
                    blog["tag_id"]=std::stoi(row[1]);
                    blog["user_id"]=std::atoi(row[2]);
                    blog["title"]=row[3];
                    blog["content"]=row[4];
                    blog["ctime"]=row[5];
                    blogs->append(blog);
                }
                mysql_free_result(res);
                return true;
            }

            bool GetTag(int tag_id,Json::Value* blogs)
            {
                int i=0;
#define SELECT_BLOG_TAG "select * from table_blog where tag_id ='%d';"
                char tmp[4096]={0};
                sprintf(tmp,SELECT_BLOG_TAG,tag_id);
                bool ret =blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                {
                    return false;
                }
                MYSQL_RES* res=mysql_store_result(_mysql);//获取结果集
                int num_row=mysql_num_rows(res);//获取行数
                if (num_row <= 0)
                {
                    printf("blog result info error\n");
                    mysql_free_result(res);
                    return false;
                }
                
                //id tag_id user_id title content ctime
                for (;i< num_row;i++)
                {
                    MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
                    Json::Value blog;
                    blog["id"]=std::stoi(row[0]);
                    blog["tag_id"]=std::stoi(row[1]);
                    blog["user_id"]=std::atoi(row[2]);
                    blog["title"]=row[3];
                    blog["content"]=row[4];
                    blog["ctime"]=row[5];
                    blogs->append(blog);
                }
                mysql_free_result(res);
                return true;
            }
         
            bool GetUser(int user_id,Json::Value* blogs)
            {
                int i=0;
#define SELECT_BLOG_USER "select * from table_blog where user_id ='%d';"
                char tmp[4096]={0};
                sprintf(tmp,SELECT_BLOG_USER,user_id);
                bool ret =blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                {
                    return false;
                }
                MYSQL_RES* res=mysql_store_result(_mysql);//获取结果集
                int num_row=mysql_num_rows(res);//获取行数
                if (num_row <= 0)
                {
                    printf("blog result info error\n");
                    mysql_free_result(res);
                    return false;
                }
                
                //id tag_id user_id title content ctime
                for (;i< num_row;i++)
                {
                    MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
                    Json::Value blog;
                    blog["id"]=std::stoi(row[0]);
                    blog["tag_id"]=std::stoi(row[1]);
                    blog["user_id"]=std::atoi(row[2]);
                    blog["title"]=row[3];
                    blog["content"]=row[4];
                    blog["ctime"]=row[5];
                    blogs->append(blog);
                }
                mysql_free_result(res);
                return true;
            }
        
            bool GetBlog(int tag_id,Json::Value* blog)
            {
                int i=0;
#define SELECT_BLOG_ONE "select * from table_blog where id ='%d';"
                char tmp[4096]={0};
                sprintf(tmp,SELECT_BLOG_ONE,tag_id);
                bool ret =blog_system::MysqlQuery(_mysql,tmp);
                if (ret == false)
                {
                    return false;
                }
                MYSQL_RES* res=mysql_store_result(_mysql);//获取结果集
                int num_row=mysql_num_rows(res);//获取行数
                if (num_row <= 0)
                {
                    printf("blog result info error\n");
                    mysql_free_result(res);
                    return false;
                }
                
                //id tag_id user_id title content ctime
                for (;i< num_row;i++)
                {
                    MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
                    (*blog)["id"]=std::stoi(row[0]);
                    (*blog)["tag_id"]=std::stoi(row[1]);
                    (*blog)["user_id"]=std::atoi(row[2]);
                    (*blog)["title"]=row[3];
                    (*blog)["content"]=row[4];
                    (*blog)["ctime"]=row[5];
                }
                mysql_free_result(res);
                return true;
            }

        private:
            MYSQL* _mysql;
    };
}
