#ifndef BOGGLEPLAYER_HPP
#define BOGGLEPLAYER_HPP

#include "baseboggleplayer.h"
#include "boggleutil.h"
#include <iostream>
#include <stack>
#include <queue>
#include <unordered_map>

using namespace std;

class BogglePlayer : public BaseBogglePlayer{

/**inheritances*/
public:
  void buildLexicon(const set<string>& word_list);
  void setBoard(unsigned int rows, unsigned int cols, string** diceArray);
  bool getAllValidWords(unsigned int minimum_word_length, set<string>* words);
  bool isInLexicon(const string& word_to_check);
  vector<int> isOnBoard(const string& word_to_check);
  void getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols);
  bool recurse_onboard(int loc, const string& word_to_check);
  
  // constructor & destructor
  BogglePlayer(): root(0), brd_flag(false), lex_flag(false) {}
  ~BogglePlayer();

  //public data 
  unsigned int ROWS;
  unsigned int COLS;  

private:
  
  //data
  Node* root;
	bool  brd_flag;
	bool  lex_flag;
  vector< vector<string> > board;
  std::unordered_map< std::string, mapvalue > mymap;


  //method
  bool trie_find(const string& str);  
  Node* trie_put(Node* curr, const string& str, int offset);
  Node* trie_find(Node* curr, const string& str, int offset);
  bool getPreWord(string prefix, Node* startNode, set<string>* wordHolder);
  void getPreWord(vector<char>& charHolder, set<string>* wordHolder, Node* curr);
  Node* isPrefix(string prefix_to_check, Node* startNode);
  void recur_prefix(int xcord, int ycord, Node* curr, vector<string>& charSeq, set<string>* word_list, unsigned int min);
  void recur_dele( Node* cur );
};

#endif
