#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdint.h>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "../include/cppjieba/Jieba.hpp"
#include "../common/public.hpp"
using std::endl;
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
namespace searcher
{

/////////////////////////////////////////////////////////////////////////////////// 
//                   索引模块内容                                                //
///////////////////////////////////////////////////////////////////////////////////  
    //正拍索引用到的节点 
    struct DocInfo
    {
        int64_t doc_id;//文档id
        string title;//文档标题
        string url;//文档的url
        string content;//文档正文
    };
    //倒排索引用到的节点
    struct Weight
    {
        int64_t doc_id;//该词对应的文档id
        int weight;//该词对应某文档的权重
        string word;//词
    };
    
    //这是一个倒排索引的拉链
typedef vector<Weight> inverted_list;

    class Index
    {
        public:
            Index();
            //提供构建索引的函数以及用户调用API接口
            
            //根据id得到正排索引 
            const DocInfo* GetDocInfo(int64_t doc_id);

            //根据关键词key获得倒排索引
            const inverted_list* GetInverted(const string key);

            //构建索引
            bool Build(const string& input_path);

            //自定义的切割接口
            static void Spilt(const string& str,const string& delimiter,vector<string>* output)           
            {
                //boost中的切割接口中，输出、输入、分隔符、是否压缩                                                               
                boost::split(*output,str,boost::is_any_of(delimiter),boost::token_compress_off);
            }
        private:
            cppjieba::Jieba jieba;
            //正排索引，用vector存储，每个文档id对应下表即可
            vector<DocInfo> forward_index;
            //倒排索引，每一个单词string对应一个vector，哈希映射关系
            unordered_map<string,vector<Weight> > inverted_index;
        private:
            DocInfo*  BuildForward(const string& line);
            void BuildInverted(const DocInfo* doc_info);
            bool Cut_Word(const string& input,vector<string>* output)
            {
                jieba.CutForSearch(input,*output);
                return true;
            }
    };
/////////////////////////////////////////
/////// 搜索模块内容////////////////////
/////////////////////////////////////////  


}//searcher
