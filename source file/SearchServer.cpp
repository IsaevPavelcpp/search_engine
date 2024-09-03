#include "../header/SearchServer.h"

TEST(TestCaseSearchServer, TestSimle)
{
    const std::vector<std::string> docs =
            {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
            };
    const std::vector <std::string> request = {"milk water", "sugar"};
    const std::vector <std::vector<RelativeIndex>> expected ={
            {
                    {2,1},
                    {0,0.7},
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
                    {2, 0.666666687},
                    {3, 0.666666687},
                    {4, 0.666666687},
                    {5, 0.666666687},
                    {6, 0.666666687},
                    {8, 0.666666687},
                    {9, 0.666666687},
                    {10, 0.666666687},
                    {11, 0.666666687},
                    {12, 0.666666687},
                    {13, 0.666666687},
                    {15, 0.666666687},
                    {16, 0.666666687},
                    {17, 0.666666687},
                    {18, 0.666666687},
                    {19, 0.666666687},
                    {20, 0.666666687},
                    {21, 0.666666687}
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}


bool my_cmp(const RelativeIndex& a, const RelativeIndex& b){ //фунция для сортировки
    return a.rank > b.rank;
};

SearchServer::SearchServer(InvertedIndex &idx) noexcept : _index(idx){};

// Метод обработки поисковых запросов
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)noexcept
{
    std::vector<std::vector<RelativeIndex>> frequency;
    std::vector<std::string> dividerStr; // разделенный поисковый запрос
    std::vector<std::map<std::string, std::vector<Entry>>> sort_queries;// хранение совпадений по поисковому запросу
    std::vector<float> Rrel; // относительная релевантность
    float Rmax; // максимальная релевантность по всем док.
    frequency.resize(queries_input.size());
    sort_queries.resize(queries_input.size());

    for (int i = 0; i < queries_input.size(); ++i) // поиск совпадений запроса в файлах
    {
        lineDivider(dividerStr, queries_input[i]);
        for (const auto& word : dividerStr) {
            sort_queries[i][word] = _index.GetWordCount(word);
        }
    }

    std::map <size_t , float> buffer;
    int counter = 0;
    for (auto& request : sort_queries)// подсчет совпадений
    {
        for (auto& word : request) {
            for (auto stack : word.second)
            {
                if (!word.second.empty()) {
                    buffer[stack.doc_id] += (float)stack.count;
                }
            }
        }
        for (const auto& iter : buffer) { // заполнение ответа
            frequency[counter].push_back({iter.first, iter.second});
        }
        buffer.clear();
        // сортировка по релевантности begin > end
        std::sort(frequency[counter].begin(),frequency[counter].end(), my_cmp);
        counter ++;
    }

    for (auto& request : frequency)
    {
        Rmax = 0;
        for (const auto& value : request) {
            Rmax = (Rmax < value.rank) ? value.rank : Rmax; // расчет абсолютной релевантности
        }
        for (auto& value : request) {
            value.rank /= Rmax;   // расчет относительной релевантности документов
        }
    }
    return frequency;
}