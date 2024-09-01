#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>
#include <string>

struct configuration
{
    std::string name = "uncnown";
    int version = 0;
    int max_resp = 0;
    std::vector <std::string> files;
}; // структура для записи конфига

struct RelativeIndex{
    size_t doc_id = 0;
    float rank = 0;

    bool operator ==(const RelativeIndex& other) const {
    return (doc_id == other.doc_id && rank == other.rank);
    }
};

class ConverterJSON
        {
    configuration config;
public:
    configuration GetTextDocuments();
    int GetResponsesLimit();
    std::vector<std::string> GetRequests();
    void putAnswers(std::vector<std::vector<RelativeIndex>>answers);
        };


#endif //SEARCH_ENGINE_CONVERTERJSON_H
