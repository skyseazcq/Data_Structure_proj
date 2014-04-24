#include <iostream> // cout
#include <fstream> 	// file acess
#include <string> 
#include <stdlib.h> // atoi
#include <set> 

using namespace std;

int main(int argc, char* argv[]){
	/**Read the brd into an array***/
	/*
	ifstream ifs;
	ifs.open(argv[1],ifstream::in);
	string tmp;	
	std::getline(ifs, tmp);
	int ROWS = atoi(tmp.c_str());
	std::getline(ifs, tmp);
	int COLS = atoi(tmp.c_str());

	string diceArray[ROWS][COLS];
	for (int i = 0; i < ROWS; i++){
		for (int j = 0; j < COLS; j++){
			std::getline(ifs, tmp);
			diceArray[i][j] = tmp;
		}
	}
	ifs.close();
	*/

	/**read the lex into a set*/
	ifstream ifs;
	ifs.open(argv[1],ifstream::in);
	string tmp;
	std::getline(ifs, tmp);
	set<string> wordlist;

	while (ifs.good()){
		wordlist.insert(tmp);
		cout << tmp << endl;
		std::getline(ifs, tmp);
	}
	cout << "size of set:" << wordlist.size() << endl;
	// read lex done
	return 0;
}
