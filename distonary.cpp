//using C++20
// 
// Distonary - english to russian -
// vers. 1.0
// 18.01.2022
// from Alexandr Ryazanov 
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <limits>
#include <numeric>
#include <sstream>
#include <fstream>
#include <Windows.h>

using namespace std;

enum class QueryType {
	AddWord,
	AllWord,
	FindWord,
	DeleteWord,
	SaveDistonary,
	Help,
	Exit,
	NoType,
};

struct Query {
	QueryType type;
	string eng_word;
	string translate;

};

istream& operator>>(istream& is, Query& q) {
	
	string operation_code;
	is >> operation_code;

	if (operation_code == "add"s) {
		q.type = QueryType::AddWord;
		string text;
		is >> q.eng_word;
		getline(cin, text);
		q.translate = text;
		
	}
	else if (operation_code == "del"s) {
		q.type = QueryType::DeleteWord;
		is >> q.eng_word;
	}
	else if (operation_code == "find"s) {
		q.type = QueryType::FindWord;
		is >> q.eng_word;
	}
	else if (operation_code == "all"s) {
		q.type = QueryType::AllWord;

	}
	else if (operation_code == "save"s) {
		q.type = QueryType::SaveDistonary;

	}
	else if (operation_code == "help"s) {
		q.type = QueryType::Help;

	}
	else if (operation_code == "exit"s) {
		q.type = QueryType::Exit;

	}
	else if (operation_code != "exit"s) {
		q.type = QueryType::NoType;
		cout << "The command is missing. Type help to find out the supported commands.  " << endl;

	}


	return is;
}
ostream& operator<<(ostream& os, const map<string, string>& dist) {

	if (dist.empty()) {
		os << "No word";
	}
	else {

		for (const auto& [word, translate] : dist) {
			os << word << ':';

			os << ' ' << translate << endl;
		}

	}
	return os;
}

class Distonary {
public:
	map<string, string> eng_to_ru_translate;
	map<string, string>::iterator it = eng_to_ru_translate.begin();

	void AddWord(string& word, string& translate) {

		if (!translate.empty()) {
			eng_to_ru_translate[word] = translate;
			cout << word << " " << eng_to_ru_translate[word] << " added" << endl;
		}
		else {
			cout << "You have not entered the transfer, repeat the operation" << endl;
		}

	}

	string GetDeletWord(const string& word) {


		if (eng_to_ru_translate.count(word)) {
			it = eng_to_ru_translate.find(word);
			eng_to_ru_translate.erase(it);

		}
		return "Слово: " + word + " удалено";
	}

	map<string, string> GetAllWord() const {

		return { eng_to_ru_translate };
	}
	map<string, string>GetFindWord(const string& word, const map<string, string>& dist) const {


		map<string, string>result = {};
		for (auto [key, value] : dist) {
			for (int i = 0; i < word.size(); i++) {

				if (word[i] == key[i] && i == word.size() - 1) {
					result.insert(pair{ key, value });
				}
				else if (word[i] == key[i]) {
					continue;
				}
				break;

			}
		}
		return result;
	}

	void SaveDataDistonary(const map<string, string>& m) {
		ofstream file;
		file.open("db_distonary.txt");
		for (auto& [key, value] : m)
		{
			file << key << " " << value << endl;
		}

		file.close();
		cout << "Data saved" << endl;
	}

	void PrintHelp() {
		cout << "   List of commands:" << endl;
		cout << "   add  [eng_word] [translate-from-rus] - add" << endl;
		cout << "   find [eng_word]                      - find word" << endl;
		cout << "   del  [eng_word]                      - delete word from distonary" << endl;
		cout << "   all                                  - output of the entire dictionary" << endl;
		cout << "   help                                 - help by commands " << endl;
		cout << "   exit                                 - exiting the program" << endl;
		cout << "--------------------------------------------------------------------------" << endl;

	}

	void PrintHeader() {
		cout << "==========================================================================" << endl;
		cout << "=========================== Distonary Programm ===========================" << endl;
		cout << "================================== MENU ==================================" << endl;
	}

	void deleteSpaces(std::string& string) {
		size_t strBegin = string.find_first_not_of(' ');
		size_t strEnd = string.find_last_not_of(' ');
		string.erase(strEnd + 1, string.size() - strEnd);
		string.erase(0, strBegin);
	}

	void SetDistonaryFromDataFile() {
		ifstream infile("db_distonary.txt");
		string key, value;
		while (infile >> key) {
			getline(infile, value);
			deleteSpaces(value);
			eng_to_ru_translate[key] = value;
		}
	}
};

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	Query q;
	Distonary dist;
	dist.PrintHeader();
	dist.PrintHelp();
	dist.SetDistonaryFromDataFile();

	do {
		cin >> q;
		switch (q.type) {
		case QueryType::AddWord:
			dist.AddWord(q.eng_word, q.translate);
			cout << "---------------------------------------------------" << endl;
			break;
		case QueryType::DeleteWord:
			cout << dist.GetDeletWord(q.eng_word) << "---------------------------------------------------" << endl;
			break;
		case QueryType::FindWord:
			cout << dist.GetFindWord(q.eng_word, dist.eng_to_ru_translate) << "---------------------------------------------------" << endl;
			break;
		case QueryType::AllWord:
			cout << dist.GetAllWord() << "---------------------------------------------------" << endl;
			break;
		case QueryType::SaveDistonary:

			dist.SaveDataDistonary(dist.eng_to_ru_translate);
			break;
		case QueryType::Help:
			dist.PrintHelp();
			break;
		case QueryType::NoType:
			cin.clear(); // на случай, если предыдущий ввод завершился с ошибкой
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			break;
		case QueryType::Exit:
			break;

		default: cout << "Error unknow";//dist.PrintHelp();
		}
	} while (q.type != QueryType::Exit);
	dist.SaveDataDistonary(dist.eng_to_ru_translate);
	cout << "See you soon :)))))";

	//system("pause");
}

