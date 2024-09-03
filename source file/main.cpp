#include "../header/SearchServer.h"


int main() {
    ::testing::InitGoogleTest();
    ConverterJSON convert_json;
    configuration config_str;
    std::vector<std::string> docs;
    config_str = convert_json.GetTextDocuments();
    docs.resize(config_str.files.size());

    for(int i = 0; i < config_str.files.size(); ++i)
    {
            std::ifstream file(config_str.files[i]);
            std::getline(file, docs[i]);
    } // чтение текстов
    InvertedIndex II;
    II.UpdateDocumentBase(docs);
    SearchServer SS(II);
    convert_json.putAnswers(SS.search(convert_json.GetRequests()));
    std::cout << clock() << "\n";
    return RUN_ALL_TESTS();;

}
