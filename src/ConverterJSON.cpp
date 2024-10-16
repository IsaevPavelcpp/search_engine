
#include "../header/ConverterJSON.h"
#include <iomanip>

//считывает конфиг из json
    configuration ConverterJSON::GetTextDocuments(configuration& config)
    {
        std::ifstream file("..\\config.json");
        if(!file.is_open())
        {
            throw "ERROR: config file is missing, The file may be stored elsewhere. The file path must be: search-engine\\config.json ";
        }

        nlohmann::json config_json;
        file >> config_json;
        file.close();
        config.name = config_json["config"]["name"];
        config.version = config_json["config"]["version"];
        max_response = config.max_resp = config_json["config"]["max_responses"];
        if(config_json["files"].empty())
        {
            throw std::string {"ERROR: file is empty\n"};
        }
        for(const auto &it:config_json["files"])
        {
            config.files.push_back(it);
        }
        return config;
    }

// получает запрос на поиск
   std::vector<std::string> ConverterJSON::GetRequests() noexcept
    {
        std::ifstream file("..\\requests.json");
        nlohmann::json request_json;
        file >> request_json;
        file.close();
        std::vector<std::string> request_str;
        for(const auto &it : request_json["requests"])
        {
            request_str.push_back(it);
        }
        return request_str;
    }

// отправляет результат работы
    void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>>
                    answers) const noexcept {
    std::cout << "We are sending a response...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    bool found;
    std::ofstream file("..\\answers.json");
    file << "{\"answers\":{" << std::endl;
    for (int i = 0; i != answers.size(); ++i) {
        file << std::right << std::setw(13) << "\"request " + std::to_string(i) << "\":{" << std::endl;
        if (answers[i].empty()) // проверка на наличие совпадений
        {
            found = false; // если совпадений нет запись result: false
            file << std::right << std::setw(15) << "\"result\":" << std::boolalpha << found << std::endl;
        }
        else
        {
            found = true;
            file << std::right << std::setw(15) << "\"result\":" << std::boolalpha << found << "," << std::endl;
            if (answers[i].size() > 1) //если совпадения есть -> проверка на кол-во совпадений
            {                         //запись relevance и перечисление совпадений
                file << std::right << std::setw(23) << "\"relevance\":{\n";
                for (int j = 0; j != answers[i].size(); ++j) {
                    if (j >= max_response) {
                        break;
                    }
                    file << std::right << std::setw(21) << "\"docid\": " << answers[i][j].doc_id << ", ";
                    if (j == answers[i].size() - 1) {
                        file << "\"rank\": " << answers[i][j].rank << std::endl;
                    } else { file << "\"rank\": " << answers[i][j].rank << "," << std::endl; }
                }
            } else {// если совпадение только одно оно записывается без relevance
                file << std::right << std::setw(21) << "\"docid\": " << answers[i][0].doc_id << ", ";
                file << "\"rank\": " << answers[i][0].rank << std::endl;
            }
            file << std::right << std::setw(12) << "}\n";
        }
        if (i == answers.size() - 1) {
            file << std::right << std::setw(7) <<"}\n";
        } else { file << std::right << std::setw(6) <<"},\n"; }
    }
    file << "}}";
    file.close();
    std::cout << "The data has been sent.\n";
}
