#include "searcher.hpp"

int main()
{
    searcher::Index index;
    bool ret =index.Build("../data/tmp/raw_input");
    if (ret == false)
    {
        cout<<"构建索引失败"<<endl;
        return 0;
    }
    auto* inverted_list=index.GetInverted("filesystem");
    for (const auto& weight : *inverted_list)
    {
        cout<<"doc_id="<<weight.doc_id<<"   weight="<<weight.weight<<endl;
        auto* doc_info=index.GetDocInfo(weight.doc_id);
        cout<<"title :"<<doc_info->title<<"   url："<<doc_info->url<<endl<<"content："<<doc_info->content<<endl;
        cout<<"-------------------------------------------------------------------------------------------"<<endl;
    }
    return 0;
}
