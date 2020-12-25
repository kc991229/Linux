#include "searcher.hpp"
using std::cerr;

namespace searcher
{
    ///////////////////////////////////////////////////////////////////////////////
    /////                      以下为索引模块函数实现                      ////////
    ///////////////////////////////////////////////////////////////////////////////
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

 //   void Index::Cut_Word(const string& input,vector<string>* output) 
 //   {    
 //       jieba.CutForSearch(input,*output);
 //   }
    DocInfo* Index:: BuildForward(const string& line)
    {
        //1.把line按照/3切割成3部分
        vector<string> tokens;
        searcher::Index::Spilt(line,"\3",&tokens);
        //public_function::Util::Spilt(line,"\3",&tokens);
        if (tokens.size()!= 3)
        {
            cout<<"切割异常size="<<tokens.size()<<endl;
        }
        //2.利用切割好的结果填充一个docInfo对象
        DocInfo info;
        info.doc_id=this->forward_index.size();
        info.title=tokens[0];
        info.url=tokens[1];
        info.content=tokens[2];
        //将info设定为右值，这样可以调用push_back的特殊接口，直接转移即将被销毁的info
        this->forward_index.push_back(std::move(info));
        //3.返回给用户，注意野指针问题
        return &forward_index.back();
    }
    void Index::BuildInverted(const DocInfo* doc_info)
    {
        //用来存储单词词频
        struct word_cnt
        {
            int title_cnt;
            int content_cnt;
            word_cnt()
            {
                title_cnt=0;
                content_cnt=0;
            }
        };
        unordered_map<string, word_cnt> word_cnt_map;

        //1.针对标题进行分词
        vector<string> title_token;
        Cut_Word(doc_info->title,&title_token );
        //2.遍历结果集，统计每个词的次数
        //要考虑将大小写问题.因此要先转小写
        for (string word: title_token)
        {
            boost::to_lower(word);
            word_cnt_map[word].title_cnt++;
        }
        //3.针对正文进行分词
        vector<string> content_token;
        Cut_Word(doc_info->content,&content_token);
        //4.遍历结果集，统计每个词的次数
        for (string word :content_token)
        {
            boost::to_lower(word);
            word_cnt_map[word].content_cnt++;
        }
        //5.根据两个结果集，构建Weight
        //word_pair用来遍历哈希表中的键值对
        for (const auto word_pair: word_cnt_map)
        {
            Weight weight;
            weight.doc_id=doc_info->doc_id;
            weight.weight=word_pair.second.title_cnt*10+word_pair.second.content_cnt;
            weight.word=word_pair.first;

            //将创建好的索引加入到倒排中
            //先找到对应的倒排拉链，随后插入
            vector<Weight>& invertedlist =inverted_index[word_pair.first];
           invertedlist.push_back(std::move(weight));
        }
    }
    bool Index::Build(const string& input_path)
    {
        //1.根据预处理模块创建的文件(raw_input)，读取内容,该文件是一个行文本文件，并用/3分割成了三块:title、url、content
        cerr<<"开始构造索引"<<endl;
        std::ifstream file(input_path);
        if (!file.is_open())
        {
            cerr<<"打开raw_input文件失败"<<endl;
            return false;;
        }
        string line;
        while (getline(file,line))
        {
            //2.根据读到的内容，填充构建正排索引
            DocInfo* doc_info=BuildForward(line);
            if (doc_info == nullptr)
            {
                cerr<<"构建正排索引失败"<<endl;
                continue;
            }

            //3.根据正排索引，构建倒排索引
            BuildInverted(doc_info);
            if (doc_info->doc_id %100 ==0)
            {
                cerr<<"已经处理:"<<doc_info->doc_id<<endl;
            }
        }
        cerr<<"索引构造结束"<<endl;
        file.close();
        return true;
    }
////////////////////////////////////////////////////////////////////////////////////////
///////                            以下为搜索模块函数实现                         //////
////////////////////////////////////////////////////////////////////////////////////////

    //初始化函数
    bool Searcher::Init(const string& input_path)
    {
        return this->index->Build(input_path);
    }
    //根据查询词进行搜索，得到结果
    bool Searcher::Search(const string& query,string* output)
    {
        //1.分词。针对查询词进行分词
        vector<string> tokens;
        index->Cut_Word(query,&tokens);
        //2.触发。根据分词结果，查倒排，获取相关文档
        vector<Weight> all_token_result;
        for (string word : tokens)
        {
            //进行大小写统一化
            boost::to_lower(word);
            auto* inverted_list =index->GetInverted(word);
            if (inverted_list == nullptr)
            {
                //若该单词在倒排索引中并不存在，得到的就是nullptr
                continue;
            }
            all_token_result.insert(all_token_result.end(),inverted_list->begin(),inverted_list->end());
        }
        //3.排序。将查找到的文档根据权重进行降序排序
        std::sort(all_token_result.begin(),all_token_result.end(),[](const Weight& w1,const Weight& w2){
                return w1.weight>w2.weight ;
                });//采用lambda表达式

        //4.包装。把得到的倒排拉链中的文档id获取，进而查正排，把doc_info中的内容构造
        //构造采用json格式
        Json::Value results;//该对象中可能包含多个搜索结果
        for (const auto& weight:all_token_result)
        {
            const DocInfo* doc_info= index->GetDocInfo(weight.doc_id);
            //将doc_info对象包装成一个json
            Json::Value result;
            result["title"]=doc_info->title;
            result["url"]=doc_info->url;
            result["desc"]=GenerateDesc(doc_info->content,weight.word);
            results.append(result);
        }

        //5.将得到的results这个JSON对象序列化成字符串，写入output
        Json::FastWriter writer;
        *output=writer.write(results);
        return true;
    }
    string Searcher::GenerateDesc(const string& content,const string& word)
    {
        //根据正文，找到word的出现位置
        //以位置为中心。向前找60个单词，作为描述的开始处
        //再从起始位置向后找100个单词，作为结束位置
        size_t frist_pos = content.find(word);
        size_t desc_beg=0;
        if (frist_pos == string ::npos)
        {
            //若该词在正文中未出现，仅仅存在于标题，以开头为初始位置
            if (content.size() <160)
            {
                return content;
            }
            string desc=content.substr(0,160);
            desc[desc.size()-1] = '.';
            desc[desc.size()-2] = '.';
            desc[desc.size()-3] = '.';
            return desc;
        }
        desc_beg = frist_pos <60? 0:frist_pos -60;
        if (desc_beg+160 >= content.size())
        {
            return content.substr(desc_beg);
        }
        else
        {
            string desc =content.substr(desc_beg,160);
            desc[desc.size()-1] = '.';
            desc[desc.size()-2] = '.';
            desc[desc.size()-3] = '.';
            return desc;
        }
    }
}//end searcher 
