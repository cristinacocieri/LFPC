#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>	//library for regular expressions
#include <random>
#include <deque>	//double-ended queue, indexed sequence container

/*Variant 5
S->dB|A
A->d|dS|aBdB
B->a|aS|AC
D->AB
C->bC|*
*/

using namespace std;

int letter = 86; // 86 - ASCII Code for letter V, starting from this, will appear in Chomsky Normal Form(V,W,X,Y,Z...)

struct PRODUCTION {
	string key;
	deque<string> dervation;
};

PRODUCTION newobject(deque<string> object) {
	PRODUCTION temp;
	temp.key = object[0];
	object.pop_front();
	temp.dervation = object;
	return temp;
}

vector<PRODUCTION> readFIle()
{
	vector<PRODUCTION> proj;
	string line;
	ifstream myfile("Grammar.txt");
	deque<string> tmp;
	regex rgx("(\\->)|(\\|)");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line == "") continue;
			sregex_token_iterator iter(line.begin(), line.end(), rgx, -1);
			sregex_token_iterator end;
			for (; iter != end; ++iter)
				tmp.push_back(*iter);
			proj.push_back(newobject(tmp));
			tmp.clear();
		}
		myfile.close();
	}
	return proj;
}

void removeNUll(vector<PRODUCTION>* ptr, int i) {
	for (int k = 0; k < (*ptr).size(); k++)
	{
		for (int j = 0; j < (*ptr)[k].dervation.size(); j++)
		{
			if (!((*ptr)[k].dervation[j].find((*ptr)[i].key) != std::string::npos)) continue;
			string tmp = (*ptr)[k].dervation[j];
			tmp.erase(std::remove(tmp.begin(), tmp.end(), (*ptr)[i].key[0]), tmp.end());
			(*ptr)[k].dervation.push_back(tmp);
		}

	}
}

void EmptyProduction(vector<PRODUCTION>* ptr) {
	for (int i = 0; i < (*ptr).size(); i++)
	{
		for (int j = 0; j < (*ptr)[i].dervation.size(); j++)
		{
			if ((*ptr)[i].dervation[j] != "*") continue;
			(*ptr)[i].dervation.erase((*ptr)[i].dervation.begin() + j);
			removeNUll(ptr, i);
		}
	}
}

void RemovingUnits(vector<PRODUCTION>* ptr, int index, string key) {
	for (int i = 0; i < (*ptr).size(); i++)
		if ((*ptr)[i].key == key)
			(*ptr)[index].dervation.insert((*ptr)[index].dervation.end(), (*ptr)[i].dervation.begin(), (*ptr)[i].dervation.end());
}

void UnitProductions(vector<PRODUCTION>* ptr) {
	for (int i = 0; i < (*ptr).size(); i++)
	{
		for (int j = 0; j < (*ptr)[i].dervation.size(); j++)
		{
			if ((*ptr)[i].dervation[j].size() != 1) continue;
			if ((*ptr)[i].dervation[j][0] > 90 || (*ptr)[i].dervation[j][0] < 65) continue;
			string tmp = (*ptr)[i].dervation[j];
			(*ptr)[i].dervation.erase((*ptr)[i].dervation.begin() + j);
			RemovingUnits(ptr, i, tmp);
		}
	}
}

void NewProductionForNonterminals(vector<PRODUCTION>* ptr, string temp) {
	PRODUCTION tmp;
	tmp.key = letter;
	letter++;
	tmp.dervation.push_back(temp.substr(1, 2));
	for (int i = 0; i < (*ptr).size(); i++)
	{
		for (int j = 0; j < (*ptr)[i].dervation.size(); j++)
		{
			if ((*ptr)[i].dervation[j].size() > 2) {
				(*ptr)[i].dervation[j].erase(1, 2);
				(*ptr)[i].dervation[j].append(tmp.key);
			}
		}
	}
	(*ptr).push_back(tmp);
}

void NewProduction(vector<PRODUCTION>* ptr, string tmp) {
	PRODUCTION temp;
	temp.key = letter;
	letter++;
	temp.dervation.push_back(tmp.substr(0, 1));
	for (int i = 0; i < (*ptr).size(); i++)
	{
		for (int j = 0; j < (*ptr)[i].dervation.size(); j++)
		{
			if ((*ptr)[i].dervation[j][0] == tmp[0] && (*ptr)[i].dervation[j].size() > 1) {
				(*ptr)[i].dervation[j][0] = temp.key[0];
			}
		}
	}
	(*ptr).push_back(temp);
}

void ChomskyNormalForm(vector<PRODUCTION>* ptr) {
	for (int i = 0; i < (*ptr).size(); i++)
	{
		for (int j = 0; j < (*ptr)[i].dervation.size(); j++)
		{
			if ((*ptr)[i].dervation[j].size() > 2)
				NewProductionForNonterminals(ptr, (*ptr)[i].dervation[j]);
			if ((*ptr)[i].dervation[j].size() > 1 && !isupper((*ptr)[i].dervation[j][0]))
				NewProduction(ptr, (*ptr)[i].dervation[j]);
		}
	}
}

int main() {
	vector<PRODUCTION> checkSome = readFIle();
	EmptyProduction(&checkSome);
	cout << "After removing empty productions:" << endl;
	for (int i = 0; i < checkSome.size(); i++)
	{
		cout << checkSome[i].key << " --> ";
		for (int j = 0; j < checkSome[i].dervation.size(); j++)
		{
			cout << checkSome[i].dervation[j] << " ";
		}
		cout << "\n";
	}
	UnitProductions(&checkSome);
	cout << "After removing unit productions:" << endl;
	for (int i = 0; i < checkSome.size(); i++)
	{
		cout << checkSome[i].key << " --> ";
		for (int j = 0; j < checkSome[i].dervation.size(); j++)
		{
			cout << checkSome[i].dervation[j] << " ";
		}
		cout << "\n";
	}
	ChomskyNormalForm(&checkSome);
	cout << "Chomsky Normal Form:" << endl;
	for (int i = 0; i < checkSome.size(); i++)
	{
		cout << checkSome[i].key << " --> ";
		for (int j = 0; j < checkSome[i].dervation.size(); j++)
		{
			cout << checkSome[i].dervation[j] << " ";
		}
		cout << "\n";
	}
	return 0;
}