//
// Created by Павел on 07.08.2024.
//

#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H
#include "../header//InvertedIndex.h"
#include "../header/ConverterJSON.h"
#include <algorithm>



class SearchServer
        {
    InvertedIndex _index;
        public:
    SearchServer(InvertedIndex& idx);
    std::vector<std::vector<RelativeIndex>>search(const std::vector<std::string>& queries_input);
};


#endif //SEARCH_ENGINE_SEARCHSERVER_H
