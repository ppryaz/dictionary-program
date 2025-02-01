//	using C++20
// 
// Dictonary - english to russian -
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
	addWord,
	AllWord,
	FindWord,
	DeleteWord,
	SaveDictonary,
	Help,
	Exit,
	NoType,
};

struct Query {
	QueryType 	type;
	string 		eng_word;
	string 		translate;
};

//--------------------------------------------------------------------

istream& operator>>(istream& is, Query& q) {
	
	string operation_code;
	is >> operation_code;

	if (operation_code == "add"s) {
		q.type = QueryType::addWord;
		string text;
		is >> q.eng_word;
		getline(cin, text);
		q.translate = text;		
	} else if (operation_code == "del"s) {
		q.type = QueryType::DeleteWord;
		is >> q.eng_word;
		
	} else if (operation_code == "find"s) {
		q.type = QueryType::FindWord;
		is >> q.eng_word;
		
	} else if (operation_code == "all"s) {
		q.type = QueryType::AllWord;

	} else if (operation_code == "save"s) {
		q.type = QueryType::SaveDictonary;

	} else if (operation_code == "help"s) {
		q.type = QueryType::Help;

	} else if (operation_code == "exit"s) {
		q.type = QueryType::Exit;

	} else if (operation_code != "exit"s) {
		q.type = QueryType::NoType;
		cout << "The command is missing. Type help to find out the supported commands.  " << endl;
	}

	return is;
}

ostream& operator<<(ostream& os, const map<string, string>& dict) {

	if (dict.empty()) {
		os << "No word";
	} else { 
		for (const auto& [word, translate] : dict) {
			
			os << word << ':';
			os << ' ' << translate << endl;
		}

	}
	return os;
}
//--------------------------------------------------------------------

class Dictonary {
private:
	map<string, string> eng_to_ru_translate;
	map<string, string>::iterator it = eng_to_ru_translate.begin();

public:
	
	void addWord(string& word, string& translate) {
		if (!translate.empty()) {
			
			eng_to_ru_translate[word] = translate;
			cout << word << " " << eng_to_ru_translate[word] << " added" << endl;
		
		} else {
			cout << "You have not entered the transfer, repeat the operation" << endl;
		}
	}
	//------------------
	
	string deleteWord(const string& word) {
		if (eng_to_ru_translate.count(word)) {
			it = eng_to_ru_translate.find(word);
			eng_to_ru_translate.erase(it);
		}
		return "Слово: " + word + " удалено";
	}
	//------------------
	
	map<string, string> getAllWord() const { return { eng_to_ru_translate }; }
	//------------------
	
	map<string, string>findWords(const string& word) const {
		const map<string, string>& dict = eng_to_ru_translate;
		map<string, string>result = {};
		
		for (auto [key, value] : dict) {
			for (int i = 0; i < word.size(); i++) {
				if (word[i] == key[i] && i == word.size() - 1) {
					result.insert(pair{ key, value });
				} else if (word[i] == key[i]) {
					continue;
				}
				break;
			}
		}		
		return result;
		
	}
	//---------------------
	
	void saveData() {
		const map<string, string>& m = eng_to_ru_translate;
		
		ofstream file;
		file.open("db_dictionary.txt");
		
		for (auto& [key, value] : m) file << key << " " << value << endl; 

		file.close();
		cout << "Data saved" << endl;
	}
	//--------------------
	
	void printHelp() {
		cout << "   List of commands:" << endl;
		cout << "   add  [eng_word] [translate-from-rus] - add" << endl;
		cout << "   find [eng_word]                      - find word" << endl;
		cout << "   del  [eng_word]                      - delete word from dictonary" << endl;
		cout << "   all                                  - output of the entire dictionary" << endl;
		cout << "   help                                 - help by commands " << endl;
		cout << "   exit                                 - exiting the program" << endl;
		cout << "--------------------------------------------------------------------------" <<endl;

	}
	//--------------------
	
	void printHeader() {
		cout << "==========================================================================" << endl;
		cout << "=========================== Dictonary Programm ===========================" << endl;
		cout << "================================== MENU ==================================" << endl;
	}
	//--------------------
	
	void loadData() {
		ifstream infile("db_dictionary.txt");
		string key, value;
		while (infile >> key) {
			getline(infile, value);
			deleteSpaces(value);
			eng_to_ru_translate[key] = value;
		}
	}
	//--------------------
	
private:
	void deleteSpaces(std::string& string) {
		size_t strBegin = string.find_first_not_of(' ');
		size_t strEnd = string.find_last_not_of(' ');
		string.erase(strEnd + 1, string.size() - strEnd);
		string.erase(0, strBegin);
	}	
};

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	Query q;
	Dictonary dict;
	dict.printHeader();
	dict.printHelp();
	dict.loadData();

	do {
		cin >> q;
		switch (q.type) {
		case QueryType::addWord:
			dict.addWord(q.eng_word, q.translate);
			cout << "---------------------------------------------------" << endl;
			break;
		case QueryType::DeleteWord:
			cout << dict.deleteWord(q.eng_word) << "---------------------------------------------------" << endl;
			break;
		case QueryType::FindWord:
			cout << dict.findWords(q.eng_word) << "---------------------------------------------------" << endl;
			break;
		case QueryType::AllWord:
			cout << dict.getAllWord() << "---------------------------------------------------" << endl;
			break;
		case QueryType::SaveDictonary:

			dict.saveData();
			break;
		case QueryType::Help:
			dict.printHelp();
			break;
		case QueryType::NoType:
			cin.clear(); // на случай, если предыдущий ввод завершился с ошибкой
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			break;
		case QueryType::Exit:
			break;

		default: cout << "Error unknow";//dict.printHelp();
		}
	} while (q.type != QueryType::Exit);
	dict.saveData();
	cout << "See you soon :)))))";

	//system("pause");
}

