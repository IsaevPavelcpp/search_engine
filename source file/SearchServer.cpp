#include "../header/SearchServer.h"

TEST(TestCaseSearchServer, TestSimple) {
    const std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const std::vector<std::string> request = {"milk water", "sugar"};
    const std::vector<std::vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.7},
                    {1, 0.3}
            },
            {
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}
TEST(TestCaseSearchServer, TestTop5) {
    const std::vector<std::string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
    };
    const std::vector<std::string> request = {"moscow is the capital of russia"};
    const std::vector<std::vector<RelativeIndex>> expected = {
            {
                    {7, 1},
                    {14, 1},
                    {0, 0.666666687},
                    {1, 0.666666687},
                    {2, 0.666666687}
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}


bool my_cmp(const RelativeIndex& a, const RelativeIndex& b)
{
    return a.rank > b.rank;
};

InvertedIndex _index;

SearchServer::SearchServer(InvertedIndex &idx){_index = idx;};

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)
{
    std::vector<std::vector<RelativeIndex>> frequency;
    std::vector <std::string> dividerStr; // разделенный поисковый запрос
    std::vector <std::map <std::string, std::vector<Entry>>> sort_queries;// хранение совпадений по поисковому запросу
    std::vector <float> Rrel; // относительная релевантность
    frequency.resize(queries_input.size());
    float Rmax = 0; // максимальная релевантность по всем док.
    sort_queries.resize(queries_input.size());

    for(int i = 0; i < queries_input.size(); ++i) // поиск совпадений запроса в файлах
    {
        lineDivider(dividerStr, queries_input[i]);
        for(int j = 0; j < dividerStr.size(); ++j)
        {
            sort_queries[i][dividerStr[j]] = _index.GetWordCount(dividerStr[j]);
        }
    }
    for(int i = 0; i < sort_queries.size(); ++i)// подсчет совпадений
    {
        std::vector<RelativeIndex> buf;
        for(auto it = sort_queries[i].begin(); it != sort_queries[i].end(); ++it)
        {
            for(int j = 0; j < it->second.size(); ++j)
            {
                buf.resize(it->second.size());
                if(it->second.size() != 0)
                {
                    buf[j].doc_id = it->second[j].doc_id;
                    buf[j].rank += it->second[j].count;
                }
            }
            frequency[i] = buf;
        }
        std::sort(frequency[i].begin(),frequency[i].end(), my_cmp);
    }

    for(int i = 0; i < frequency.size(); ++i)// расчет релевантности
    {
        Rmax = 0;
        for(int j = 0; j < frequency[i].size(); ++j)
        {
            Rmax = (Rmax < frequency[i][j].rank) ? frequency[i][j].rank : Rmax;
        }
        for(int j = 0; j < frequency[i].size(); ++j)
        {
            frequency[i][j].rank/=Rmax;
        }
    }
    return frequency;
}