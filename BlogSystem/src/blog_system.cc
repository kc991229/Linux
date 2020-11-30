#include "db.hpp"
int main()
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
    return 0;
}
