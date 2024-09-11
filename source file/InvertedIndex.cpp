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
TEST(TestCaseInvertedIndex, TestEmptyRequest)
{
    const std::vector<std::string> docs = {
            "a b c d e f g h i j k l",
            "statement spell such",
            "It’s origins and the history of creation are still hotly debated in scholarly circles"
            "The events unfolding on a tapestry took place in the years 1064 to 1066"
    };
    const std::vector<std::string> requests = {" "};
    const std::vector<std::vector<Entry>> expected = {
            {}
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

std::shared_mutex mtx;

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

//принимает запросы и делит каждый на отдельные слова, в отельных потоках запускает поиск
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)noexcept {
    std::cout << "Preparation of documents...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    docs.clear();
    docs.resize(input_docs.size());
    std::vector<std::thread> threads;
    for (int j = 0; j < docs.size(); ++j) {
        lineDivider(docs[j], input_docs[j]);
        std::cout << "Progress: " << 100 / (docs.size() - j) << "%\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        threads.emplace_back(&InvertedIndex::CountMatch, this);
    }
    for (int j = 0; j < docs.size(); ++j) {
        threads[j].join();
    }
}
//поиск совпадений
void InvertedIndex::CountMatch()noexcept
{
    std::lock_guard<std::shared_mutex> lock(mtx);
    for (auto& word : docs[i]) {
        if (freq_dictionary.count(word)) {
            if (i > freq_dictionary[word].size() - 1) {
                freq_dictionary[word].push_back({(size_t )i,1});
            } else if (i == freq_dictionary[word][i].doc_id) {
                freq_dictionary[word][i].count++;
            }
        } else if (!freq_dictionary.count(word) && word != " ") {
            freq_dictionary[word].push_back({(size_t )i,1});
        }
    }
    i++;
}

//позволяет извлечь отдельное слово из словаря
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)noexcept
{
    return freq_dictionary[word];
}
