#ifndef BOGGLEUTIL_HPP
#define BOGGLEUTIL_HPP

/** the following class does not need any function
 *  to be defined in .c
 */

typedef struct{
	int location:24;
	int count: 8;
} mapvalue;

class Node{
public:
	char key;
	Node* left;
	Node* right;
	Node* middle;
	bool end;

	//constructor
	Node(char key_in): key(key_in), left(0), right(0), middle(0), end(false){}
};

#endif