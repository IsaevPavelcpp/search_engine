Search Engine

Поисковый движок в виде консольного приложения, который
осуществляет поиск и имеющее возможность настройки через файлы формата
JSON. 

В проекте разработаны 3 класса:

1)ConverterJSON (Считывает файл конфигурации и поисковый запрос, записывает финальный ответ)
	Методы:
	1 - GetTextDocuments (Считывает config.json)
	2 - GetRequests (Считывает request.json)
	3 - putAnswers (Запоняет answers.json)

2)InvertedIndex(Подготавливает загруженные тексты, формирует частотный словарь)
	Методы:
	1 - UpdateDocumentBase(принимает запросы и делит каждый на отдельные слова, в отельных потоках запускает поиск)
	2 - CountMatch (поиск совпадений)
	3 - GetWordCount (позволяет извлечь отдельное слово из словаря)

3)SearchServer (Производит расчеты по подготовленному частотному словарю)
	SearchServer(InvertedIndex &idx) noexcept : _index(idx){}; (в конструктор класса передаётся ссылка на класс
InvertedIndex, чтобы SearchServer мог узнать частоту слов встречаемых в запросе)
	Search Метод обработки поисковых запросов, поэтапно запускает методы для поиска совпадений (void SearchServer::searchMatches(const std::vector<std::string>& queries_input)), посчета совпадений (void SearchServer::countingMatches()),  расчета релевантности(void SearchServer::relevanceCalculation()). Возвращает подготовленный список для дальнейшей записи в файл answers.json.
	

В проекте используется:
1) nlohmann::json (библиотека, которая используется для генерации JSON-файлов в C++).
2) Google C++ Testing Framework (GTest, GMock)(Для проведения
автоматических модульных тестов).
Всего в проекте 6 тестов. 4 для класса InvertedIndex и 2 для класса searchServer.
3) В проекте используется обработка исключений.
   Возможные:
   1) class ConverterJson:
   1. ERROR: config file is missing(ошибка возникает при отсутвии файла конфигурации)
   2. ERROR: file is empty(ошибка возникает если список документов в файле конфигурации пустой)
   2) class SearchServer
   1. WARN: A comma was found in the request, please list the request only separated by a space(ошибка возникает при попытки перечислить запрос через запятую.
      Все слова в запросе должны быть разделены только пробелом)
   2. FATAL: division by zero(ошибка может возникнуть только в результате сбоя программы, при котором будет деление на 0)


	  
