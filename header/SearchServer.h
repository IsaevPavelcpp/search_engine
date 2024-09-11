#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#include "../header//InvertedIndex.h"
#include "../header/ConverterJSON.h"
#include <algorithm>

class SearchServer {
    InvertedIndex _index;
    std::vector<std::vector<RelativeIndex>> frequency;
    std::vector<std::string> dividerStr; // разделенный поисковый запрос
    std::vector<std::map<std::string, std::vector<Entry>>> sort_queries;// хранение совпадений по поисковому запросу
    std::vector<float> Rrel; // относительная релевантность
    float Rmax = 0; // максимальная релевантность по всем док.
public:
    explicit SearchServer(InvertedIndex &idx) noexcept;
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queries_input) noexcept;
    void searchMatches(const std::vector<std::string> &);
    void countingMatches();

    void relevanceCalculation();
};


#endif //SEARCH_ENGINE_SEARCHSERVER_H
