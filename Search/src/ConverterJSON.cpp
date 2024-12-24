#include <fstream>
#include <vector>
#include <exception>
#include "ConverterJSON.h"
#include "nlohmann/json.hpp"
#include "Configuration.cpp"
#include "Entry.cpp"
#include <thread>
#include <mutex>

using namespace std;

class ConverterJSON {
public: ConverterJSON() = default;
	  Configuration configuration;
	  vector<string> getRequests;

	  //функция начала работы движка
	  void start()
	  {
		  cout << "                              Launching a search engine... " << "\n";
		  nlohmann::json config;
		  ifstream cnfile("config.json");
		  if (!cnfile)
		  {
			  throw exception();
		  }
		  for (int i = 0; i < ConfigurationCounter<Configuration>::getConfigurationCounter(); i++)
		  {
			  cnfile >> config;
		  }
		  config["config"]["maxResponses"] = { MAX_RESPONS };
		  cout << config["config"] << "\n";
		  for (const auto& item : config["config"].items())
		  {
			  if (item.key() == "name")
			  {
				  configuration.name = item.value();
			  }
			  else if (item.key() == "version")
			  {
				  configuration.version = item.value();
			  }		  
		  }
		  int count = 0;
		  for (const auto& item : config["files"].items())
		  {
			  int id = stoi(item.key());
			  pair<int, string> oPair(id, item.value());
			  configuration.movieTitles.insert(oPair);
			  count++;
		  }
		  cnfile.close();
	  };


	  //функция проверки наличия файла
	  bool FileIsExist(string filePath)
	  {
		  bool isExist = false;
		  std::ifstream fin(filePath.c_str());

		  if (fin.is_open())
			  isExist = true;

		  fin.close();
		  return isExist;
	  }

	  //функция проверки открытия файла
	  void openFile(string path)
	  {
		  if (FileIsExist(path))
		  {
			  ofstream file;
			  file.open(path, std::ofstream::out | std::ofstream::trunc);
			  file.close();
		  }
		  else
		  {
			  ofstream reqFile(path);
			  reqFile.close();
		  }
	  };

	  //функция ввода запроса
	  void requerysInputFunction(vector<string>& getRequests)
	  {
		  if(getRequests.size()< configuration.maxRequest)
		  {
		  cout << "\n" << "                                      Information about domestic and foreign films" << "\n" << "\n";
		  cout << "               Search query field" << "\n" << "\n";
		  string requerie = "";
		  getline(cin, requerie);
		  if (requerie.length() > configuration.maxStrRequestLength)
			  requerie.erase(requerie.length() - configuration.maxStrRequestLength);
		  getRequests.push_back(requerie);
		  requerie = "";
		  ofstream reqfile("requests.json");
		  nlohmann::json reqconfig;
		  reqconfig["number"] = getRequests.size();
		  reqconfig["request"] = getRequests[getRequests.size() - 1];
		  reqfile << reqconfig;
		  reqfile.close();
	      }
	  }

	  //функция выделения слов
	  void wordSplitFunction(string sentence, vector<string>& setWords)
	  {
		  string highlightedWord = "";
		  int j = 0, k = 0;
		  for (int ir = 0; ir < sentence.length(); ++ir)
		  {
			  if (sentence[ir] != '-' && sentence[ir] != '\'' && sentence[ir] != '/'  && sentence[ir] != ')' && sentence[ir] != '(' && sentence[ir] != ' ' && sentence[ir] != ';' && sentence[ir] != ':' && sentence[ir] != ',' && sentence[ir] != '.')
			  {
				  if (sentence[ir] >= 65 && sentence[ir] <= 90)
				  {
					  sentence[ir] += 32;
				  }
				  highlightedWord += sentence[ir];
				  j++;
		   }
			  else
			  {
				  setWords.push_back(highlightedWord);
				  highlightedWord = "";
				  j = 0;
			  }
			  if(ir==sentence.length()-1)
				  setWords.push_back(highlightedWord);
		  }
	  }

	  //функция подсчёта слов в документе
	 multimap<string, vector< Entry>> countWordsMap;
	  void wordCountFunction(multimap<string, vector< Entry>>& countWMap)
	  {
		  static const int dcN = configuration.documentsNumber;
		  vector<string> vectorWord;
		  vector<Entry> getWordCount;
		  string strWord = "";
		  int filesCount = 0;
		  vector<Entry> vectorEntry;
#pragma omp parallel for
			  for (auto it = configuration.movieTitles.begin(); it != configuration.movieTitles.end(); ++it)
			  {
				  string moviePath = it->second;
				  ifstream file;
				  file.open(moviePath);
				  file.seekg(0);
				  if (!file.is_open())
				  {
					  cerr << "\n" << "The file is not found." << "\n";
				  }
				  while (getline(file, strWord))
				  {
					  wordSplitFunction(strWord, vectorWord);
				  }
				  file.close();
				  size_t wordRepetitionCount = 0;
				  int entryCount = 0;
				  vector<string> checkRepetition;
#pragma omp parallel for
				  for (int itr = 0; itr < vectorWord.size(); ++itr)
				  {

					  Entry* entry = new Entry[MAX_SIZE];
					  entry[entryCount].docId = filesCount;
					  for (int iter = itr + 1; iter < vectorWord.size(); ++iter)
					  {
						  if (vectorWord[itr] == vectorWord[iter])
						  {
							  wordRepetitionCount++;
						  }
					  }
					  wordRepetitionCount++;
					  auto pointer = find(checkRepetition.begin(), checkRepetition.end(), vectorWord[itr]);
					  if ((pointer == end(checkRepetition)) && (vectorWord[itr].size() > 3))
					  {
						  entry[entryCount].freqWordsCount = wordRepetitionCount;
						  entry[entryCount].word = vectorWord[itr];
						  getWordCount.push_back(entry[entryCount]);
					  }
					  checkRepetition.push_back(vectorWord[itr]);
					  wordRepetitionCount = 0;
					  ++entryCount;
					  delete[entryCount] entry;
				  }
				  checkRepetition.clear();
				  filesCount++;
				  vectorWord.clear();
				  for (int ip = 0; ip < getWordCount.size(); ++ip)
				  {
					  int matchСount = 0;
					  for (int iq = 0; iq < getWordCount.size(); ++iq)
					  {
						  if ((getWordCount[ip].word == getWordCount[iq].word) && (matchСount == 0))
						  {
							  vectorEntry.push_back(getWordCount[iq]);
							  matchСount++;
						  }
					  }
					  countWMap.insert({ getWordCount[ip].word, vectorEntry });
					  vectorEntry.clear();
				  }
				  getWordCount.clear();
			  }
	  }

	  //функция поиска ответов. Здесь ведётся поиск в контейнере.
	  void searchAnswerFunction(multimap<string, vector< Entry>>& countWMap)
	  {
		  vector<string> requestWord;
		  ifstream requestFile("requests.json");
		  if (!requestFile)
			  cout << "\n " << "The file is not found.";
		  nlohmann::json configr;
			  requestFile >> configr;
		  requestFile.close();
#pragma omp parallel for
			  for (const auto& iterator : configr["number"].items())//добавленный цикл
			  {
				  string strRequest = configr["request"];
			  wordSplitFunction(strRequest, requestWord);
			  multimap<size_t, size_t> searchResult;
			  ofstream ansfile("answers.json");
			  nlohmann::json ansconfig[MAX_RESPONS], ansconfigur, answerconfig, answconfig;
			  int absoluteRelevance = 0,
				  maxAbsoluteRelevanceDoc = 0,
				  maxAbsoluteRelevance = 0,
				  nlohmArrayCount = 0;
			  answerconfig = { {"request", iterator.value()} };//добавлено
			  for (int i = 0; i < requestWord.size(); ++i)
			  {
#pragma omp parallel for
				  for (auto it = countWMap.begin(); it != countWMap.end(); ++it)
				  {
					  if (requestWord[i] == it->first)
					  {
						  for (auto m : it->second)
						  {
							  searchResult.insert({ m.freqWordsCount, m.docId });
							  absoluteRelevance += m.freqWordsCount;
							  if (absoluteRelevance > maxAbsoluteRelevance)
								  maxAbsoluteRelevanceDoc = absoluteRelevance;
						  } // закрывает цикл по auto m : it->second
					  }
				  }//закрывает auto it = countWMap
				  int countResponses = 0;
				  for (auto iter = searchResult.crbegin(); iter != searchResult.crend(); ++iter)
				  {
					  if (countResponses < MAX_RESPONS)
					  {
						  if (iter->first > 0)
						  {
							  ansconfigur += { { "Document number", iter->second }, { "Number of matches",iter->first }, };
						  }
					  }
					  else
					  {
						  searchResult.clear();
						  break;
					  }
					  countResponses++;
				  } //закрывает цикл по searchResult.cr
				  if (absoluteRelevance > 0)
					  ansconfig[nlohmArrayCount] = { { "word ", requestWord[i]},{"result", { absoluteRelevance > 0} }, {"Absolute relevance", absoluteRelevance},{"Relative relevance",0.}, { ansconfigur} };
				  if (absoluteRelevance == 0)
				  {
					  ansconfig[nlohmArrayCount] = { { "word ", requestWord[i]}, {"result", { absoluteRelevance != 0}}, {"Absolute relevance", absoluteRelevance},{"Relative relevance",0.}, { ansconfigur == 0} };
				  }
				  nlohmArrayCount++;
				  ansconfigur.clear();
				  absoluteRelevance = 0;
				  searchResult.clear();
				  if (maxAbsoluteRelevanceDoc > maxAbsoluteRelevance)
					  maxAbsoluteRelevance = maxAbsoluteRelevanceDoc;
			  } //закрывает i < requestWord.size()
			  double relativeRelevance = 0;
			  for (int id = 0; id < MAX_RESPONS; ++id)
			  {
				  for (const auto& item : ansconfig[id].items())
				  {
					  relativeRelevance = (double)ansconfig[id][2].at(1) / (double)maxAbsoluteRelevance;
					  relativeRelevance = round(relativeRelevance * 100) / 100;
					  ansconfig[id][3].at(1) = relativeRelevance;
					  cout << "\n " << item.value();
				  }
				  cout << "\n";
				  if (ansconfig[id] > 0)
					  answconfig += {{ansconfig[id]}};//добавлено
			  }
			  answerconfig = { {"request",  configr["number"]},{answconfig}};//добавлено
			  ansfile << answerconfig;//добавлено
			  answerconfig.clear();//добавлено
		       searchResult.clear();
		       ansconfig->clear();
		       ansfile.close();
			   strRequest = "";
			   requestWord.clear();
	      } //закрывает auto& item : configr["number"].items
	  }// выход из функции
};//закрывает класс
