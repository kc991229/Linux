#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

using std::string;
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
    };
}//public public_function 
