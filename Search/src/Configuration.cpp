#include "Configuration.h"
#include <iostream>
#include <string>
#include <map>
#include "ConfigurationCounter.cpp"
#define MAX_SIZE 10000 //���������� ����� ���������� ���� � ����������
#define MAX_RESPONS 5 //���������� ����� �������

using namespace std;

template <class T> int ConfigurationCounter <T>::Counter(0);
struct Configuration :ConfigurationCounter <Configuration>
{
	string name;//�������� �������� ������
	string version;//������ ������
	map<int, string> movieTitles;
	static const int maxRequest = 1000;//������������ ���������� �������
	static const int documentsNumber = 10; //���������� ����������
	static const int maxStrRequestLength = 80; //���������� ���������� ������ � �������
};