#include "boggleplayer.h"
#include <random>    // std::default_random_engine
#include <chrono>    // std::chrono
#include <algorithm>
#include <ctype.h>

/** all the global variables*/
int board_rows,  board_cols, dice_maxlen;
int precount = 0;
int tcount = 0;
int dx[] = {0, 1, 1, 1, 0, -1, -1, -1},
    dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
vector<int> flag; // location has already been included if 1;
vector<int> globvector;
vector<vector<int> > preflag;
/**implementation of method
 * whenever we call build, the trie gets rebuilt
	*/

void
BogglePlayer::buildLexicon(const set<string>& word_list){
	// set the flag
	this->lex_flag = true;
	
	/**Since the set is ordered, which would cost unbalaced trie
	 * the first thing we would do is to shuffle it */
	vector<string> word_vector;
	//cout << "before inserting into word_vector: size = " << word_vector.size() << endl;
	for ( std::set<string>::iterator it0 = word_list.begin(); it0 != word_list.end(); ++it0){
					word_vector.push_back( *it0 );
	}

	// now all the words in vector, begin the shuffle
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle (word_vector.begin(), word_vector.end(), std::default_random_engine(seed) );

	for (vector<string>::iterator it = word_vector.begin(); it != word_vector.end(); ++it){
					//cout << ' ' << *it;
					root = trie_put(root, *it, 0); // this recursive call would put the entire string into the tri
	}
	//cout <<"build lex done" <<  endl;
}

// trie_put returns the pointer
Node*
BogglePlayer::trie_put(Node* curr, const string& str, int offset){
	char tmp = str.at(offset);

	if (curr == 0) 	curr = new Node(tmp);
	if 				(tmp < curr->key) 					curr->left   = trie_put(curr->left, str, offset);
	else if   (tmp > curr->key) 					curr->right  = trie_put(curr->right, str, offset);
	else if 	(offset < str.length() - 1) curr->middle = trie_put(curr->middle, str, ++offset);
	else 																	curr->end    = true;
	return curr;
}

// public find
bool
BogglePlayer::trie_find(const string& str){
	Node* last_char = trie_find(root, str, 0);
	if (last_char == 0) return false;
	return last_char->end;
}


// private find
Node*
BogglePlayer::trie_find(Node* curr, const string& str, int offset){
	if (curr == 0) return 0;
	char tmp = str.at(offset);

	if 			(tmp < curr->key) 						return trie_find(curr->left, str, offset);
	else if (tmp > curr->key) 						return trie_find(curr->right, str, offset);
	else if (offset < str.length() - 1)		return trie_find(curr->middle, str, ++offset);
	else 																	return curr;
}


void
BogglePlayer::setBoard(unsigned int rows, unsigned int cols, string **diceArray){
	
	this->brd_flag = true;

  board.resize(rows, vector<string>(cols));
  std::unordered_map<std::string, mapvalue>::iterator got;
  board_rows = rows;
  board_cols = cols;
  dice_maxlen = 0;
	this->ROWS = rows;
	this->COLS = cols;
	for(int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			// all chars to lower case if necessary
			string tmp = diceArray[i][j];
			string allLower;
			for ( int k = 0; k < tmp.size(); k++){
				allLower.push_back( tolower(tmp[k]) );
			}	
			//cout << "size of all lower is:" << allLower.size() << endl;
			board[i][j] = allLower;

			if (dice_maxlen < allLower.length()) dice_maxlen = allLower.length();
            got = mymap.find(allLower);
            if (got == mymap.end()) {
                mapvalue tmpvalue;
                tmpvalue.location = i*cols + j;
                tmpvalue.count = 1;
                mymap[allLower] = tmpvalue;
            }
            else {
                got->second.count += 1;
                //cout << got->second.count <<endl;
            }
		}
	}
}

vector<int>
BogglePlayer::isOnBoard(const string& word_to_check) {
    vector<int> dice_loc;
		//sanity check: if setBoard is not called, return empty vector
		if ( this->brd_flag == false ) return dice_loc;

    stack<mapvalue> lifo_check; // empty
    int dice_strloc, dice_str_x, dice_str_y, loc_count;
    int maxlen_check = (dice_maxlen < word_to_check.length()) ? dice_maxlen : word_to_check.length();
    std::unordered_map<std::string, mapvalue>::iterator got;
    for (int i = 1; i < maxlen_check + 1; i++) {
        string sub = word_to_check.substr(0, i);
        got = mymap.find(sub);
        if (got != mymap.end()) {
            //cout << "sub:  " << sub << endl;
           // cout << "location:  " << got->second.location <<endl;
            lifo_check.push(got->second);
	    
        }
    }

    if(lifo_check.size() == 0) {
        cout << "not on board!" << endl;
        return dice_loc;
    }
    //cout << "stack size: " << lifo_check.size() << endl;
    flag.resize(board_rows*board_cols);
    globvector.clear();
    do {
        dice_strloc = lifo_check.top().location;
        dice_str_x = dice_strloc/board_cols;
        dice_str_y = dice_strloc%board_cols;//dice_strloc - dice_str_x*board_cols;
        loc_count = lifo_check.top().count;
        do {
            //cout << "dice_strloc: " << dice_strloc <<endl;
            flag.clear();
            //cout << "board[dice_str_x][dice_str_y]: " << board[dice_str_x][dice_str_y] <<endl;
            string next_word = word_to_check.substr(board[dice_str_x][dice_str_y].length(), word_to_check.length() - board[dice_str_x][dice_str_y].length());

						if ( next_word == "" ){
							dice_loc.push_back(dice_strloc);
							return dice_loc;
						}

           // cout << "next_word:  " << next_word << endl;
            bool b = recurse_onboard(dice_strloc, next_word);
            if (b == true) {
                dice_loc.push_back(dice_strloc);
                for(int i = globvector.size() - 1; i >= 0; i--) {
                    dice_loc.push_back(globvector[i]);
                }
                //globvector.clear();
                return dice_loc;
            }
            loc_count--;
            if (loc_count > 0) {
                dice_strloc++;
                while(dice_strloc < board_rows*board_cols) {
                    int x =  dice_strloc/board_cols;
                    int y =  dice_strloc%board_cols;//dice_strloc - x*board_cols;
                    if (board[x][y] == board[dice_str_x][dice_str_y]) {
                        dice_str_x = x;
                        dice_str_y = y;
                        break;
                    }
                    dice_strloc++;
                }
            }
        } while(loc_count > 0);
        lifo_check.pop();
    } while (lifo_check.size() > 0);
    return dice_loc;
    /*for (int i = 0; i < 8; i++) {
        next_loc = dice_strloc + dx[i] + board_cols*dy[i];
        if (next_loc < 0 || next_loc > board_rows*board_cols -1) continue;
        int k = next_loc/board_cols;
        string ss = board[k][next_loc - k*board_cols];
    }*/

}

bool
BogglePlayer::recurse_onboard(int loc, const string& word_to_check) {
    flag[loc] = 1;
    int x = loc/board_cols;
    int y = loc%board_cols;
    for (int i = 0; i < 8; i++) {
        int next_loc = loc + dx[i] + board_cols*dy[i];
        if (flag[next_loc] == 1 || x+dy[i]<0 ||x+dy[i]>=board_rows|| y+dx[i]<0 || y+dx[i]>=board_cols) continue;
        int k = next_loc/board_cols;
        string ss = board[k][next_loc%board_cols];//[next_loc - k*board_cols];
        if (word_to_check.find(ss) != (std::size_t)0) continue;
        if (word_to_check.length() == ss.length()) {
            globvector.push_back(next_loc);
            return true;
        }
        //cout << "ss: " << ss <<endl;
        string next_word = word_to_check.substr(ss.length(), word_to_check.length() - ss.length());
        //cout << "next_word:  " << next_word << endl;
        bool b = recurse_onboard(next_loc, next_word);
        if (b == true) {
            globvector.push_back(next_loc);
            return true;
        }
    }
    flag[loc] = 0;
    return false;
}
// startNode should be pointing at the last digit in the prefix
bool
BogglePlayer::getPreWord(string prefix, Node* startNode, set<string>* wordHolder){

	if (startNode->middle == 0) return false;
	vector<char> charHolder;
	for (int i = 0; i < prefix.length(); i++) {
        charHolder.push_back(prefix[i]);
    }
	getPreWord(charHolder, wordHolder, startNode->middle);
	return true;
}

// private function
void
BogglePlayer::getPreWord(vector<char>& charHolder, set<string>* wordHolder, Node* curr){
	if (curr == 0) return;

	//step1
	getPreWord(charHolder, wordHolder, curr->left );

	//step2
	charHolder.push_back(curr->key);
	if (curr->end == true){
		// turn all the characters in the charHolder to a string
		string wordFound(charHolder.begin(), charHolder.end());
		//cout << "wordFound: " << wordFound << " " << wordFound.length() << endl;
		wordHolder->insert(wordFound);
		precount++;

	}
	getPreWord(charHolder, wordHolder, curr->middle);
	charHolder.pop_back();

	//step3
	getPreWord(charHolder, wordHolder, curr->right);
}


Node*
BogglePlayer::isPrefix(string prefix_to_check, Node* startNode){
	Node* curr = startNode;
	for ( int i = 0; i < prefix_to_check.size(); i++){
		char digit = prefix_to_check.at(i);
		int flag = 0;

		while(curr != 0){
			if 			( digit < curr->key) curr = curr->left;
			else if ( digit > curr->key) curr = curr->right;
			else{
				if (i != prefix_to_check.size() - 1){
					flag = 1;
					curr = curr->middle;
					break;
				}
				else return curr;
			}
		}
		if (flag == 0) return 0; // not found
	  else 					continue;
	}
	cout << "this should never happen" << endl;
	return curr;
}

/*destructor of the BogglePlayer
 * mainly erase the ternary trie
 */
BogglePlayer::~BogglePlayer(){
	//cout << "calling the destructor" << endl;
	recur_dele(root->left);
	recur_dele(root->middle);
	recur_dele(root->right);
	delete root;
	root = 0;
}

// helper function for destructor
void
BogglePlayer::recur_dele( Node* cur ){
	if ( cur!= 0 ){
		recur_dele(cur->left);
		recur_dele(cur->middle);
		recur_dele(cur->right);
		delete cur;
	}
	cur = 0;
	//else: we return
}


bool
BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words) {
	// sanity check
	if ( this->brd_flag == false || this->lex_flag == false) return false;

	for ( int i = 0; i < COLS*ROWS; i++){
		/**clear is not resetting!! it removes everything from the vector
		 * resetting the vector*/
		flag.clear(); 
		flag.resize(COLS*ROWS, 0);

		string tmp_string = board[i/COLS][i%COLS];
		Node* dice = isPrefix(tmp_string , root );
		if ( dice != 0 ){
			if ( dice->end == true && tmp_string.size() >= minimum_word_length ){
				words->insert(tmp_string);
			}
			flag[i] = 1;
			vector<string> charArray;
			charArray.push_back(tmp_string);
			recur_prefix( i/COLS, i%COLS, dice, charArray, words, minimum_word_length);
		}
	}
	return true;
}
			
void
BogglePlayer::recur_prefix(int xcord, int ycord, Node* curr, vector<string>& charSeq, set<string>* word_list, unsigned int min){
	if ( curr->middle == 0 ){}
	else{
		for (int i = 0; i < 8; i++){
			int newx = xcord+dx[i];
			int newy = ycord+dy[i];
			int flagindex = newx*COLS + newy;
			// for each neighbor of the curr node
			if ( newx >= 0 && newx < ROWS && newy >= 0 && newy < COLS && flag[flagindex] == 0 ){
				Node* newStart = isPrefix( board[newx][newy], curr->middle );
				if ( newStart == 0 ){}  // the string is not in the subtree
				else{
					charSeq.push_back( board[newx][newy] );
					flag[flagindex] = 1;

					if ( newStart->end == 1 ){
						string tmp;
						for (auto it = charSeq.begin(); it != charSeq.end(); ++it){
							tmp.append( *it );
						}
						if ( tmp.size() >= min ) word_list->insert(tmp); // add the new word to container
					}
	
					recur_prefix ( newx, newy, newStart, charSeq, word_list, min );
					charSeq.pop_back();
					flag[flagindex] = 0;
				}
			}
		}
	}
}

bool
BogglePlayer::isInLexicon(const string &word_to_check){
	// sanity check
	if ( this->lex_flag == false ) return false;
	return trie_find(word_to_check);
}

void
BogglePlayer::getCustomBoard(string** & new_board, unsigned int *rows, unsigned int *cols){
	 	//string* new_brd[3];
		//string* new_brd = new string*[3];
		string** new_brd = new string*[3];
		for ( int i = 0; i < 3 ; i++){
			string* col_ptr = new string[4];
			new_brd[i] = col_ptr;
			for ( int j = 0; j < 4; j++){
				new_brd[i][j] = "a";
			}
		}

		*rows = 3;
		*cols = 4;
		new_board = new_brd;
		/*
		for ( int i = 0; i < 3; i++){
			for ( int j = 0; j < 4; j++){
				cout << new_board[i][j] << " ";
			}
			cout << endl;
		}
		*/
}
