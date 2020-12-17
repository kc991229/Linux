#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "../common/public.hpp"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;

//用来表示从什么路径读取
string g_input_path="../data/input";
//用来表示输出到哪里
string g_output_path="../data/tmp/raw_input";

//创建一个结构体，用来存放读取到的内容
//解析的最终目的就是填充这个结构体
struct DocInfo
{
    string title;//标题
    string url;//url
    string content;//正文
};

//枚举所有的文件路径
bool EnumFile(const string& inputpath,vector<string>* file_list)
{
    //方便后续的使用
    namespace fs=boost::filesystem;

    fs::path root_path(inputpath);//递归遍历需要使用的一个类，可以用string构造
    if(!fs::exists(root_path))
    {
        cout<<"当前目录不存在"<<endl;
        return false;
    }

    fs::recursive_directory_iterator end_it;//把迭代器生成的默认构造函数当作一个哨兵
    //使用上面的root_path配合迭代器遍历,该迭代器具备自动递归访问该目录下所有文件的功能
    for (fs::recursive_directory_iterator it(root_path);it!=end_it;it++)
    {
        //1.当前路径里面是一个文件夹，不做处理
        if (!fs::is_regular_file(*it))//该接口用来判断是否为一个普通的文件
        {
            continue;
        }
        //2.当前路径里的文件不是html文件，不做处理
        if (it->path().extension() != ".html")
        {
            continue;
        }

        //添加进返回参数中
        file_list->push_back(it->path().string());
    }
    return true;
}
//在<>里的内容为标签，需要进行去标签操作
bool Parse_content(const string& html,string* res_str)
{
    //1.循环遍历整个string
    //2.判断当前内容是否为标签内容
    //3.若为标签去除，若为正文，保存
    bool is_content=true;
    for (auto v: html)
    {
        if (is_content)//当前已经是正文
        {
            if(v == '<')
            {

                is_content=false;
            }
            else
            {
                if (v == '\n')
                {
                    v=' ';
                }
                res_str->push_back(v);
            }
        }
        else//当前为标签内容
        {
            if (v == '>')
            {
                //意味着标签结束了
                is_content=true;
            }
        }
    }
    return true;
}
//用来寻找某文件的标题
bool Parse_title(const string& all_str,string* res_str)
{
    //1.寻找string内的<title> </title>的下标
    size_t head=0,tail=0;
    head=all_str.find("<title>");
    if (head == string::npos)
    {
        return false;
    }
    head+=7;
    tail=all_str.find("</title>");
    if (tail == string::npos)
    {
        return false;
    }
    //2.以两个下标为界，之间的就是标题
    string tmp;
    while(head<tail)
    {
        tmp+=all_str[head];
        head++;
    }
    //3.将标题填充入要返回的string中
    *res_str=tmp;
    return true;
}
bool Parse_url(const string& filepath,string* res_str)
{
    //在线路径的前缀+本地路径的后半部分
    string urlhead="https://www.boost.org/doc/libs/1_53_0/doc/html";
    string urltail=filepath.substr(g_input_path.size());

    *res_str=urlhead+urltail;
    return true;
}
bool Parse_file(const string& filepath,DocInfo* info)
{
    //1.先将所有内容全部读取出来
    public_function::Util us;
    string all_html;
    int ret=us.Read(filepath,&all_html);
    if (ret == false)
    {
        cout<<"读取内容失败"<<endl;
        return false;
    }
    //2.从第一步获得的内容中提取出标题title
    ret =Parse_title(all_html,&info->title);
    if (ret == false)
    {
        cout<<"获取正文失败"<<endl;
        return false;
    }
    //3.从第一步获得的内容中提取出url
    ret =Parse_url(filepath,&info->url);
    if (ret == false)
    {
        cout<<"获取url失败"<<endl;
        return false;
    }
    //4.从第一步获得的内容中提取出content
    ret =Parse_content(all_html,&info->content);
    if (ret == false)
    {
        cout<<"获取正文失败"<<endl;
        return false;
    }
    return true;
}
//将内容写入文件。
//这里的ofstream类没有拷贝构造，因此传入的时候必须要传引用，且不能为const类型，否则无法进行操作
void WriteOutput(DocInfo& doc,std::ofstream& ostream)
{
    ostream<<doc.title<<"\3"<<doc.url<<"\3"<<doc.content<<std::endl;
}
int main()
{
    //1.得到所有文件路径
    vector<string> path_enum;
    int ret=EnumFile(g_input_path,&path_enum);
    if (ret == false)
    {
        cout<<"获取路径失败"<<endl;
        return 0;
    }
    //2.对枚举出来的路径进行解析，得到所有的解析数据,要针对每个文件单独处理
    std::ofstream outputfile(g_output_path.c_str());
    if (!outputfile.is_open())
    {
        std::cout<<"打开生成文件失败"<<endl;
        return 1;
    }
    for (auto& file_path: path_enum)
    {
        cout<<file_path<<endl;
        DocInfo doc;
        ret =Parse_file(file_path,&doc);
        if (ret == false) 
        {
            cout<<"解析该文件失败:"<<file_path<<endl;
            continue;
        }
        //3.将结果写入最终的输出文件parse_file中去
        WriteOutput(doc,outputfile);
    }
    outputfile.close();
    
    return 0;
}
