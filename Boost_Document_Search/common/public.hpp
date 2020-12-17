#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <boost/algorithm/string.hpp>
using std::string;
using std::vector;
namespace public_function
{
    class Util
    {
        public:
            //从指定路径中读取所有的内容,存放在一个string中返回
            static bool Read(const string& path_file,string* output)
            {
               std::ifstream file(path_file.c_str());
               if (!file.is_open())
               {
                   std::cout<<"error"<<std::endl;
                   return false;
               }
               //一行一行读，读取到后添加到要返回的string中
               std::string line;
               while( std::getline(file,line))
               {
                   *output += (line+"\n");

               }
               file.close();
               return true;
            }
        public:
            static void Spilt(const string& str,const string& delimiter,vector<string>* output)
                
    {
        //boost中的切割接口中，输出、输入、分隔符、是否压缩
        boost::split(*output,str,boost::is_any_of(delimiter),boost::token_compress_off);
    }
    };
}//public public_function 
