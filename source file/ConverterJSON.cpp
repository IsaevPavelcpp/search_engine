
#include "../header/ConverterJSON.h"
#include <iostream>
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>
#include <string>



    configuration config;

    configuration ConverterJSON::GetTextDocuments()//считывает конфиг из json
    {
        std::ifstream file("..\\config.json");
        nlohmann::json config_json;
        file >> config_json;
        file.close();
        config.name = config_json["config"]["name"];
        config.version = config_json["config"]["version"];
        config.max_resp = config_json["config"]["max_responses"];
        for(const auto &it:config_json["files"])
        {
            config.files.push_back(it);
        }
        return config;
    }

    int ConverterJSON::GetResponsesLimit()
    {
        return config.max_resp;
    }

    std::vector<std::string> ConverterJSON::GetRequests()// получает запрос на поиск
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

    void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>>
                    answers)// отправляет результат работы
    {
        nlohmann::json answer;
        bool found;
        std::ofstream file("..\\answers.json");
        for (int i = 0; i < answers.size(); ++i)
        {
            if(answers[i].size() == 0 )
            {
                found = false;
                answer["answers"]["request " + std::to_string(i)]["result"] = found;
            }
            else
            {
                found = true;
                answer["answers"]["request " + std::to_string(i)]["result"] = found;
                if(answers[i].size() > 1 )
                {
                    for(int j = 0; j < answers[i].size(); ++j)
                    {
                        answer["answers"]["request " + std::to_string(i)]["relevance"] +={
                                {"docid ", answers[i][j].doc_id},
                                { "rank ",answers[i][j].rank}};
                        std::cout << answers[i][j].rank << "\n";
                    }
                }
                else
                {
                    answer["answers"]["request " + std::to_string(i)]["docid "] = answers[i][0].doc_id;
                    answer["answers"]["request " + std::to_string(i)]["rank "] = answers[i][0].rank;
                }
            }
        }
        file << answer;

        file.close();
    }
