#include "../header/InvertedIndex.h"

void TestInvertedIndexFunctionality(
        const std::vector<std::string>& docs,
        const std::vector<std::string>& requests,
        const std::vector<std::vector<Entry>>& expected
) {
std::vector<std::vector<Entry>> result;
InvertedIndex idx;
idx.UpdateDocumentBase(docs);
for(auto& request : requests) {
std::vector<Entry> word_count = idx.GetWordCount(request);
result.push_back(word_count);
}
ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {
const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
};
const std::vector<std::string> requests = {"london", "the"};
const std::vector<std::vector<Entry>> expected = {
        {
                {0, 1}
        },
        {
                {0, 1}, {1, 3}
        }
};
TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
};
const std::vector<std::string> requests = {"milk", "water", "cappuccino"};
const std::vector<std::vector<Entry>> expected = {
        {
                {0, 4}, {1, 1}, {2, 5}
        }, {
                {0, 3}, {1, 2}, {2, 5}
        }, {
                {3, 1}
        }
};
TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const std::vector<std::string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const std::vector<std::string> requests = {"m", "statement"};
    const std::vector<std::vector<Entry>> expected = {
            {
            },
            {
                    {1, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
std::mutex mtx;
void  lineDivider(std::vector<std::string>& docs, const std::string& input_docs)
{
    std::string word;
    std::stringstream ss(input_docs);
    docs.clear();
    while (getline(ss, word, ' '))
    {
        docs.push_back(word);
    }
}


void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
        docs.clear();
        docs.resize(input_docs.size());
        std::vector <std::thread>threads;
        for (int j = 0; j < docs.size(); ++j)
        {
            lineDivider(docs[j], input_docs[j]);
            threads.emplace_back(std::thread(&InvertedIndex::countMatch, this));
        }
        for(int j = 0; j < docs.size(); ++j)
        {
            threads[j].join();
        }
}

void InvertedIndex::countMatch()
{
    std::lock_guard<std::mutex> lock(mtx);

    Entry buf;
    for (int j = 0; j < docs[i].size(); ++j) {
        if (freq_dictionary.count(docs[i][j])) {
            if (i > freq_dictionary[docs[i][j]].size() - 1) {
                freq_dictionary[docs[i][j]].push_back({(size_t )i,1});
            } else if (i == freq_dictionary[docs[i][j]][i].doc_id) {
                freq_dictionary[docs[i][j]][i].count++;
            }
        } else if (!freq_dictionary.count(docs[i][j]) && docs[i][j] != " ") {
            freq_dictionary[docs[i][j]].push_back({(size_t )i,1});
        }
    }
    i++;
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)
{
    return freq_dictionary[word];
}
