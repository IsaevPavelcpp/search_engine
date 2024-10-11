#include "../header/InvertedIndex.h"



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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
