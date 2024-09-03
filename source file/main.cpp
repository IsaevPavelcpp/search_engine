#include "../header/SearchServer.h"


int main() {
    ::testing::InitGoogleTest();
    ConverterJSON convert_json;
    configuration config_str;
    int enter;
    //проверка исключений config файла
    try {
        convert_json.GetTextDocuments(config_str);
    }
    catch (const std::string &empty_file) {
        std::cerr << empty_file << std::endl;
        return 0;
    }
    catch (const char *no_config) {
        std::cerr << no_config << std::endl;
        return 0;
    }

    std::vector<std::string> docs; //для хранения текстов
    docs.resize(config_str.files.size());

//чтение текстов
    for (int i = 0; i < config_str.files.size(); ++i) {
        std::ifstream file(config_str.files[i]);
        std::getline(file, docs[i]);
    }
    InvertedIndex II;
    II.UpdateDocumentBase(docs);

    std::cout << "1 - Test\n2 - Start\n3 - Exit\n";
    std::cin >> enter;
    if (enter == 1) {
        return RUN_ALL_TESTS();
    } else if (enter == 2) {
        SearchServer SS(II);
        convert_json.putAnswers(SS.search(ConverterJSON::GetRequests()));

    } else {
        return 0;
    }

}
