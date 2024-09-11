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

//функция для сортировки
bool my_cmp(const RelativeIndex& a, const RelativeIndex& b){
    return a.rank > b.rank;
};

SearchServer::SearchServer(InvertedIndex &idx) noexcept : _index(idx){};

//метод поиска совпадений
void SearchServer::searchMatches(const std::vector<std::string>& queries_input)
{
    for (int i = 0; i < queries_input.size(); ++i)
    {
        std::cout << "Progress: " << 100/(queries_input.size()-i) << "%\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lineDivider(dividerStr, queries_input[i]);//разделение запроса на отдельные слова
        for (const auto& word : dividerStr)
        {
            if(word.back() == ',' || word.front() == ',')
            {
                throw std::string {"WARN: A comma was found in the request, please list the request only separated by a space"};
            }
            sort_queries[i][word] = _index.GetWordCount(word);//поиск совпадений в словаре
        }
    }
}

//метод посчета совпадений
void SearchServer::countingMatches()
{
    std::cout << "counting coincidences...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::map <size_t , float> buffer;
    int counter = 0;

    for (auto& request : sort_queries) //проход по запросам
    {
        for (auto& word : request) // проход по словам запроса
        {
            for (auto stack : word.second)//проход по совпадениям слова в документах
            {
                if (!word.second.empty()) {
                    buffer[stack.doc_id] += (float)stack.count;
                }
                else{break;}
            }
        }

        for (const auto& iter : buffer) { // заполнение ответа
            frequency[counter].push_back({iter.first, iter.second});
        }
        buffer.clear();
        // сортировка по релевантности begin > end
        std::sort(frequency[counter].begin(),frequency[counter].end(), my_cmp);
        std::cout << "progress: " << 100/(frequency.size()-counter) << "%\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter ++;
    }
}
//метод расчета релевантности
void SearchServer::relevanceCalculation()
{
    std::cout << "consider relevance...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for (auto& request : frequency)
    {
        Rmax = 0;
        for (const auto& value : request) {
            Rmax = (Rmax < value.rank) ? value.rank : Rmax; // расчет абсолютной релевантности
        }
        for (auto& value : request) {
            if(Rmax == 0)
            {
                throw std::string {"FATAL: division by zero"};
            }
            value.rank /= Rmax;   // расчет относительной релевантности документов
        }
    }
}

// Метод обработки поисковых запросов
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)noexcept
{
    frequency.resize(queries_input.size());
    sort_queries.resize(queries_input.size());
    try
    {
        std::cout << "Matches are being searched for...\n";
        searchMatches(queries_input); // поиск совпадений запроса в файлах
    }
    catch (const std::string &find_comma)
    {
        std::cerr << find_comma << "\n";
    }
    countingMatches(); // подсчет совпадений

    try
    {
        relevanceCalculation();//расчет релевантности
    }
   catch (const std::string &division_by_zero)
   {
       std::cerr << division_by_zero << "\n";
   }
    return frequency;
}