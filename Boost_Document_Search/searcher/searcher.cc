#include "searcher.hpp"
#include "../common/public.hpp"


#include "../common/public.hpp"
namespace searcher
{
    const char* const DICT_PATH = "../jieba_dict/jieba.dict.utf8";
    const char* const HMM_PATH = "../jieba_dict/hmm_model.utf8";
    const char* const USER_DICT_PATH = "../jieba_dict/user.dict.utf8";
    const char* const IDF_PATH = "../jieba_dict/idf.utf8";
    const char* const STOP_WORD_PATH = "../jieba_dict/stop_words.utf8";
    Index::Index()
        :jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
    {}
    const DocInfo* Index::GetDocInfo(int64_t doc_id)
    {
        if (doc_id <0 || doc_id > forward_index.size())
        {
            return nullptr;
        }
        return &forward_index[doc_id];
    }

    const inverted_list* Index::GetInverted(const string key)
    {
        //调用map中的find接口根据key查询value，若找到返回对用对象，否则返回end对象
        auto it =inverted_index.find(key);
        if (it == inverted_index.end())
        {
            return nullptr;
        }
        return &it->second;
    }

    DocInfo* Index:: BuildForward(const string& line)
    {
        //1.把line按照/3切割成3部分
        vector<string> tokens;
        public_function::Util::Spilt(line,"\3",&tokens);
        if (tokens.size()!= 3)
        {
            cout<<"切割异常size="<<tokens.size()<<endl;
        }
        //2.利用切割好的结果填充一个docInfo对象
        DocInfo info;
        info.doc_id=forward_index.size();
        info.title=tokens[0];
        info.url=tokens[1];
        info.content=tokens[2];
        //将info设定为右值，这样可以调用push_back的特殊接口，直接转移即将被销毁的info
        forward_index.push_back(std::move(info));
        //3.返回给用户，注意野指针问题
        return &forward_index.back();
    }
    void BuildInverted(const DocInfo* doc_info)
    {
        //1.针对标题进行分词
        //2.遍历结果集，统计每个词的次数
        //3.针对正文进行分词
        //4.遍历结果集，统计每个词的次数
        //5.根据两个结果集，构建Weight
    }
    void Index::CutWord(const string& input,vector<string>* output)
    {
        jieba.CutForSearch(input,*output);
    }
    bool Index::Build(const string& input_path)
    {
        //1.根据预处理模块创建的文件(raw_input)，读取内容,该文件是一个行文本文件，并用/3分割成了三块:title、url、content
        cout<<"开始构造索引"<<endl;
        std::ifstream file(input_path);
        if (!file.is_open())
        {
            cout<<"打开raw_input文件失败"<<endl;
            return false;;
        }
        string line;
        while (getline(file,line))
        {
            //2.根据读到的内容，填充构建正排索引
            DocInfo* doc_info=BuildForward(line);
            if (doc_info == nullptr)
            {
                cout<<"构建正排索引失败"<<endl;
                continue;
            }
            //3.根据正排索引，构建倒排索引
            BuildInverted(doc_info);
        }
        cout<<"索引构造结束"<<endl;

        return true;
    }

    //自定义切割接口，参数为输入字符、分隔符、结果
    static void Spilt(const string& str,const string& delimiter,vector<string>* output)
    {
        //boost中的切割接口中，输出、输入、分隔符、是否压缩
        boost::split(*output,str,boost::is_any_of(delimiter),boost::token_compress_off);
    }
}//end searcher 
