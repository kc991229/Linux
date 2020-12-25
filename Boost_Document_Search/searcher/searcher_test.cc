#include "searcher.hpp"

int main()
{
    searcher::Searcher us;
    bool ret=us.Init("../data/tmp/raw_input");
    if (!ret)
    {
        cout<<"初始化失败"<<endl;
        return 0;
    }
    while (1)
    {
        cout<<"search ->"<<std::flush;
        string query;
        std::cin>>query;
        if (!std::cin.good())
        {
            std::cout<<"godbye"<<endl;
            break;
        }
        string results;
        us.Search(query,&results);
        cout<<results<<endl;
    }
    return 0;
}
