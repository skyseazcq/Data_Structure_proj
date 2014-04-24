#include "boggleplayer.h"
#include <iostream>
#include <fstream> 	// file acess
#include <stdlib.h> // atoi
#include <time.h>

int main(int argc, char* argv[]) {
    BogglePlayer* bb = new BogglePlayer();


		// get the new brd
		ifstream ifs;

		//string choice(argv[1]);
		//if (  choice == "ref") ifs.open("brdref.txt",ifstream::in);
		ifs.open("brd.txt",ifstream::in);
		string tmp;	

		std::getline(ifs, tmp);
		unsigned int ROWS = atoi(tmp.c_str());
		//bb->ROWS = ROWS; 

		std::getline(ifs, tmp);
		unsigned int COLS = atoi(tmp.c_str());
		//bb->COLS = COLS;
	
		// creat a new string**
		string* diceArray[ROWS];
		for ( int i = 0; i < ROWS ; i++){
			string* col_ptr = new string[COLS];
			diceArray[i] = col_ptr;
		}

		for (int i = 0; i < ROWS; i++){
			for (int j = 0; j < COLS; j++){
				std::getline(ifs, tmp);
				diceArray[i][j] = tmp;
			}
		}

		ifs.close();
		bb->setBoard( ROWS, COLS, diceArray);

		/** print the board
		for( int i = 0; i < ROWS; i++){
			for ( int j = 0; j < COLS; j++){
				cout<< bb->board[i][j] << " " ;
			}
			cout << endl;
		}
		*/

		// build the lex tri
		//if ( choice == "ref") ifs.open("lexref.txt",ifstream::in);
		ifs.open("lex.txt",ifstream::in);

		std::getline(ifs, tmp);
		set<string> wordlist;
	
		while (ifs.good()){
			wordlist.insert(tmp);
			//cout << tmp << endl;
			std::getline(ifs, tmp);
		}
		ifs.close();

		cout << "size of set:" << wordlist.size() << endl;
		bb->buildLexicon( wordlist );

		// try to find all words
		set<string> results;
		clock_t t;
		t = clock();
		bb->getAllValidWords(1, &results );
		t = clock() -t;
		cout<< "time: " << float(t)/CLOCKS_PER_SEC<< endl;

		// final test
		cout << results.size() << endl;

		/*
		//using custom board method
		string** custom_brd;
		unsigned int cus_row;
		unsigned int cus_col;


		bb->getCustomBoard(custom_brd,&cus_row,&cus_col);

		cout << "printing the returned brd" << endl;
		cout << custom_brd[0][0] << endl;
		cout << custom_brd[0][1] << endl;
		cout << custom_brd[0][2] << endl;
		cout << custom_brd[0][3] << endl;
		cout << custom_brd[1][1] << endl;
		*/


		/*
		for ( int i = 0; i < *cus_row; i++){
			for ( int j = 0; j < *cus_col; j++){
				cout << custom_brd[0][0];
			}
			cout << endl;
		}
		*/

		/*
		for (auto it = results.begin(); it!= results.end(); it++){
			cout << *it << endl;
		}
		*/
		
		/**memory management*/

		//step1
		delete bb;

		//step2: erase the dicearray
		for ( int i = 0; i < ROWS; i++){
			delete[] diceArray[i];
		}
		//cout << "before return " <<endl;

    return 0;
}
