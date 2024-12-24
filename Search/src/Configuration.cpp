#include "Configuration.h"
#include <iostream>
#include <string>
#include <map>
#include "ConfigurationCounter.cpp"
#define MAX_SIZE 10000 //наибольшее общее количество слов в документах
#define MAX_RESPONS 5 //наибольщее число ответов

using namespace std;

template <class T> int ConfigurationCounter <T>::Counter(0);
struct Configuration :ConfigurationCounter <Configuration>
{
	string name;//Содержит название движка
	string version;//версия движка
	map<int, string> movieTitles;
	static const int maxRequest = 1000;//максимальное количество зпросов
	static const int documentsNumber = 10; //количество документов
	static const int maxStrRequestLength = 80; //наибольшее количество знаков в запросе
};