#include "../header/SearchServer.h"


int main() {
    ConverterJSON convert_json;
    configuration config_str;
    InvertedIndex II;

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
        if(!file.is_open())
        {
            std::cerr << "attention: the file was not opened, the wrong path may have been specified\n";
            std::cerr << "==" << config_str.files[i] << "==\n";
            std::cerr << "You can continue, but the result may not meet expectations.\n";
        }
        else
        {
            std::getline(file, docs[i]);
        }
    }
        II.UpdateDocumentBase(docs);
        SearchServer SS(II);
        convert_json.putAnswers(SS.search(ConverterJSON::GetRequests()));
    return 0;
}
