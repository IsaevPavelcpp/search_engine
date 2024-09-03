#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#include "../header//InvertedIndex.h"
#include "../header/ConverterJSON.h"
#include <algorithm>

class SearchServer
        {
    InvertedIndex _index;
        public:
    explicit SearchServer(InvertedIndex& idx)noexcept;
    std::vector<std::vector<RelativeIndex>>search(const std::vector<std::string>& queries_input)noexcept;
};


#endif //SEARCH_ENGINE_SEARCHSERVER_H
