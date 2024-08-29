
#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H

#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include "gtest/gtest.h"

struct Entry {
    size_t doc_id = 0, count = 0;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const
    {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

void  lineDivider(std::vector<std::string>& docs, const std::string& input_docs);

class InvertedIndex
{
public:
    void UpdateDocumentBase(std::vector<std::string> input_docs);
    std::vector<Entry> GetWordCount(const std::string& word);
};


#endif //SEARCH_ENGINE_INVERTEDINDEX_H
