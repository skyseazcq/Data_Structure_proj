#include "boggleplayer.hpp"
#include <random>    // std::default_random_engine
#include <chrono>    // std::chrono
#include <algorithm>

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
boggleplayer::buildLexicon(const set<string>& word_list){
	
	/**Since the set is ordered, which would cost unbalaced trie
	 * the first thing we would do is to shuffle it */
	vector<string> word_vector;
	cout << "before inserting into word_vector: size = " << word_vector.size() << endl;
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
	cout <<"build lex done" <<  endl;
}

// trie_put returns the pointer
Node*
boggleplayer::trie_put(Node* curr, const string& str, int offset){
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
boggleplayer::trie_find(const string& str){
	Node* last_char = trie_find(root, str, 0);
	if (last_char == 0) return false;
	return last_char->end;
}


// private find
Node*
boggleplayer::trie_find(Node* curr, const string& str, int offset){
	if (curr == 0) return 0;
	char tmp = str.at(offset);

	if 			(tmp < curr->key) 						return trie_find(curr->left, str, offset);
	else if (tmp > curr->key) 						return trie_find(curr->right, str, offset);
	else if (offset < str.length() - 1)		return trie_find(curr->middle, str, ++offset);
	else 																	return curr;
}


void
boggleplayer::setBoard(unsigned int rows, unsigned int cols, string **diceArray){

	board.resize(rows, vector<string>(cols));
    std::unordered_map<std::string, mapvalue>::iterator got;
    board_rows = rows;
    board_cols = cols;
    dice_maxlen = 0;
	for(int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			string tmp = diceArray[i][j];
			board[i][j] = tmp;
			if (dice_maxlen < tmp.length()) dice_maxlen = tmp.length();

            got = mymap.find(tmp);
            if (got == mymap.end()) {
                mapvalue tmpvalue;
                tmpvalue.location = i*cols + j;
                tmpvalue.count = 1;
                //mymap.insert(tmp, tmpvalue);
                mymap[tmp] = tmpvalue;
            }
            else {
                got->second.count += 1;
                //cout << got->second.count <<endl;
            }
            // in map: string, first appear location, count;
			/*mapvalue tmp_map = mymap[tmp];
			tmp_map.location = i*rows + cols;
			tmp_map.count    = tmp_map.count + 1;
			mymap[tmp] 			 = tmp_map;*/

			/*
			int haha = mymap[tmp];
			haha = (haha & 0xFF) + 1;
		 	mymap[tmp]  = ((i * rows + j) << 8) | haha;
			*/

		}
	}
}

vector<int>
boggleplayer::isOnBoard(const string& word_to_check) {
    vector<int> dice_loc;
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
boggleplayer::recurse_onboard(int loc, const string& word_to_check) {
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
boggleplayer::getPreWord(string prefix, Node* startNode, set<string>* wordHolder){

	if (startNode->middle == 0) return false;
	vector<char> charHolder;
	/*for (std::string::iterator it = prefix.begin(); it != prefix.end(); ++it){
		charHolder.push_back( *it ); // iterator is of type char
	}*/
	for (int i = 0; i < prefix.length(); i++) {
        charHolder.push_back(prefix[i]);
    }
    /*for (int i = 0; i < charHolder.size(); i++) {
        cout << charHolder[i] << endl;
    }*/
	getPreWord(charHolder, wordHolder, startNode->middle);
	return true;
}

// private function
void
boggleplayer::getPreWord(vector<char>& charHolder, set<string>* wordHolder, Node* curr){
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
boggleplayer::isPrefix(string prefix_to_check, Node* startNode){

	Node* curr = startNode;
	for ( int i = 0; i < prefix_to_check.size(); i++){
		char digit = prefix_to_check.at(i);
		//cout << digit << endl;
		int flag = 0;

		while(curr != 0){
		    //cout << "key: " << curr->key << endl;
			if 			( digit < curr->key) curr = curr->left;
			else if ( digit > curr->key) curr = curr->right;
			else{
				flag = 1;
				if (i != prefix_to_check.size() - 1) curr = curr->middle;
				break; // get out of while
			}
		}

		if (flag == 0) return 0;
	  else 					continue;
	}

	//assert( prefix_to_check.at(i-1) == curr->key );
    //cout << "last key:  "<< curr->key<< endl;
	return curr;
}

bool
boggleplayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words) {
    Node* tmp;
    preflag.resize(board_rows, vector<int>(board_cols));
    //preflag.clear();
    for(int i = 0; i < board_rows; i++) {
        for (int j= 0; j < board_cols; j++) {
	   // cout << " board[i][j] " << i << " " << j << ":" << board[i][j]<<endl;
	    tmp = isPrefix(board[i][j], root);
            if ( tmp != 0 && tmp->middle !=0 ) recurse_prefix(i, j, board[i][j], tmp->middle, minimum_word_length, words);     

      }
    }
}

void
boggleplayer::recurse_prefix(int i, int j, string prestring, Node* pre, unsigned int minimum_length, set<string>* words) {
    Node* tmp;
    string tmpstr;
    preflag[i][j] = 1;
    for(int k = 0; k < 8; k++) {
	//cout<< "k: " <<k << "tcount: "<< tcount++ <<endl;
	if ( i+dy[k]<0 || i+dy[k]>=board_rows || j+dx[k]<0 || j+dx[k]>=board_cols||preflag[i+dy[k]][j+dx[k]] == 1) continue;
	//cout << "board[ik][jk] " << i+dy[k] <<" "<<j+dx[k]<<":" <<board[i+dy[k]][j+dx[k]]<<endl;
	//cout << "pre: " <<pre<<endl;
	tmp = isPrefix(board[i+dy[k]][j+dx[k]],pre);
	//cout <<"tmp: "<< tmp << "  " << "pre: "<< pre<< endl;
	if( tmp == 0 ) continue;
	tmpstr = prestring;
	tmpstr.append(board[i+dy[k]][j+dx[k]]);
	if( tmp->end == true && tmpstr.length() >= minimum_length ) {
		//cout<<"insert: " << tmpstr<< endl;
		words->insert(tmpstr); /// insert
	}
	if(tmp->middle!=0)recurse_prefix(i+dy[k], j+dx[k], tmpstr, tmp->middle, minimum_length, words);
    }
    preflag[i][j] = 0;
    //cout << "return "<<endl;
}
/*
bool
boggleplayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words) {
    // when to return false... not have called setonBoard and buildLexicon yet.
    queue<Node*> dice_node_q;
    queue<int> dice_loc_q;
    set<string> non_repeat_prefix;
    int getpre_flag = 1;
    int count = 0;
    int lexcount = 0;
    set<string> wordInLex;
    for(int i = 0; i < board_rows; i++){
	for (int j = 0; j < board_cols; j++) {
	    Node* tmp = isPrefix(board[i][j], root);
            if(tmp != 0) {
                if (tmp->end == true && board[i][j].length() >= minimum_word_length) {
			count++;
			words->insert(board[i][j]);
		}
                dice_node_q.push(tmp);
                dice_loc_q.push(i*board_cols + j);
            }
	}
    }

    cout << "count: " << count << endl;
    while(!dice_loc_q.empty()) {
        int tmploc = dice_loc_q.front();
        Node* tmpnode = dice_node_q.front();
        dice_loc_q.pop();
        dice_node_q.pop();
        int x = tmploc/board_cols;
        int y = tmploc%board_cols;
        
	//cout << "x,y: " << board[x][y] << endl;
        //getPreWord(board[x][y], tmpnode, words_check);

	//using set for uniqueness and fastness
	int size = board[x][y].length();
	string subb;
	getpre_flag = 1;
	for (int i = 1; i <= size; i++) {
	   subb = board[x][y].substr(0, i);
	   if (non_repeat_prefix.find(subb) != non_repeat_prefix.end()) {
		getpre_flag = 0;
		break;
	   }
	}
	if (getpre_flag == 1) {
		non_repeat_prefix.insert(board[x][y]);
        	getPreWord(board[x][y], tmpnode, &wordInLex);
	}
        for( int i = 0; i < words_check.size(); i++) {
        cout << words_check[i]  << " " <<words_check[i].length()<< endl;
    }*/
  /*  }
	//int total_count = 0;
	for( auto it = wordInLex.begin(); it != wordInLex.end(); ++it){
				//cout << "word to check:" << *it << endl;
          // total_count++;
            if( (*it).length() >= minimum_word_length && isOnBoard(*it).size() > 0 ) {
		words->insert( *it );
		//lexcount++;
            //cout << "TT  "<< endl;
        }
    }
    //cout << endl;
    cout << "lexcount: " << lexcount << endl;
    cout << "totalcount: " << total_count << endl;
    cout << "precount: " << precount << endl;
    return true;
}*/
