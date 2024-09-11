
#include "../header/ConverterJSON.h"


//считывает конфиг из json
    configuration ConverterJSON::GetTextDocuments(configuration& config)
    {
        std::ifstream file("..\\config.json");
        if(!file.is_open())
        {
            throw "ERROR: config file is missing, The file may be stored elsewhere. The file path must be: ..\\config.json ";
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
    file << "{\n\t\"answers\":{" << std::endl;
    for (int i = 0; i != answers.size(); ++i) {
        file << "\t\t\"request " + std::to_string(i) << "\":{" << std::endl;
        if (answers[i].empty()) {
            found = false;
            file << "\t\t\t\"result\":" << std::boolalpha << found << std::endl;
        } else {
            found = true;
            file << "\t\t\t\"result\":" << std::boolalpha << found << "," << std::endl;
            if (answers[i].size() > 1) {
                file << "\t\t\t\"relevance\":{\n";
                for (int j = 0; j != answers[i].size(); ++j) {
                    if (j >= max_response) {
                        break;
                    }
                    file << "\t\t\t\t\"docid\": " << answers[i][j].doc_id << ", ";
                    if (j == answers[i].size() - 1) {
                        file << "\"rank\": " << answers[i][j].rank << std::endl;
                    } else { file << "\"rank\": " << answers[i][j].rank << "," << std::endl; }
                }
            } else {
                file << "\t\t\t\"docid\": " << answers[i][0].doc_id << ", ";
                file << "\"rank\": " << answers[i][0].rank << std::endl;
            }
            file << "\t\t\t}\n";
        }
        if (i == answers.size() - 1) {
            file << "\t\t}\n";
        } else { file << "\t\t},\n"; }
    }
    file << "\t}\n }";
    file.close();
    std::cout << "The data has been sent.\n";
}
