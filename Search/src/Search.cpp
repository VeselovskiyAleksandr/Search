// Search.cpp: определяет точку входа для приложения.
//
#include <string>
#include "ConverterJSON.cpp"
#include "Search.h"

int main()
{
	ConverterJSON converterJSON;
	try
	{
		converterJSON.start();
	}
	catch (const exception& x)
	{
		cerr << "The file is not found.";
		return 1;
	}
	mutex request;
	request.lock();
	converterJSON.openFile("requests.json");
	request.unlock();
	request.lock();
	converterJSON.openFile("answers.json");
	request.unlock();
	request.lock();
	converterJSON.requerysInputFunction(converterJSON.getRequests);
	request.unlock();
	request.lock();
	converterJSON.wordCountFunction(converterJSON.countWordsMap);
	request.unlock();
	request.lock();
	converterJSON.searchAnswerFunction(converterJSON.countWordsMap);
	request.unlock();
	return 0;
}
