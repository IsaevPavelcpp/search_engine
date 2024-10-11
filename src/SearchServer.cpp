#include "../header/SearchServer.h"


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
    std::cout << "Counting coincidences...\n";
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
        std::cout << "Progress: " << 100/(frequency.size()-counter) << "%\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter ++;
    }
}
//метод расчета релевантности
void SearchServer::relevanceCalculation()
{
    std::cout << "Consider relevance...\n";
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