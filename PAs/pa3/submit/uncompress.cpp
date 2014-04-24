// Yue Fan <yuf011@ucsd.edu>, Chaoqun Zhu <c9zhu@ucsd.edu>
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include <fstream>
#include <queue>

using namespace std;

// function declaration
HCNode* restruct_tree(HCNode* root, queue< pair<byte, int> >& leaf_queue);


int main(int argc, char* argv[]){
	
	if(argc < 3){
		cout << "Wrong Format!! USE: ./uncompress infile outfile" << endl;
		return 0;
	}

	ifstream sourceFile;
	ofstream targetFile;

	sourceFile.open(argv[1], ios::binary);
	targetFile.open(argv[2], ios::binary);

	if ( !sourceFile.good() ){
		cout << "failed to open file!" << cout;
		return 0;
	}

	// attach our BitIO to it
	BitInputStream source(sourceFile);
	BitOutputStream target(targetFile);

	// read necessary info from the sourceFile
	
	// 1. read the total number of bytes
	int totalByte = source.readInt();

	// 2. raed the total number of unique chars
	// it returns an int, but in the range of 0 - 255
	int diffChar  = source.readByte();

	// 3. iteratively read out (symbol, level), push the pair into queue
	queue< pair<byte,int> > leaf_queue;

	while ( diffChar != 0 ){
		int  symbol =  source.readByte();	
		int  level  =  source.readByte();
		pair <byte, int> tmp_pair = make_pair(symbol, level);
		leaf_queue.push(tmp_pair);

		// decrement diffChar, 0 terminates queue construction
		diffChar --;
	}

	// call function to reconstruct hufftree
	HCNode* root = new HCNode(0,0);
	root = restruct_tree(root, leaf_queue);

	// declare a hufftree, to use its destructor
	HCTree hufftree;
	hufftree.setroot(root);
	
	// start the decoding
	HCNode* currNode = root;
	int bit = source.readBit();

	// outer while: total chars test
	while ( totalByte != 0 ){

		// inner while: leaf test
	  while ( currNode->c0 != 0 || currNode->c1 != 0){
			if( bit == 0 )
				currNode = currNode->c0;
			else
				currNode = currNode->c1;
			
			// read a new bit
			bit = source.readBit();
		}

		// now currNode is pointing at the corresponding leaf
		int symbol = int(currNode->symbol) & 0xFF;
		target.writeByte(symbol);

		totalByte --;
		currNode = root;
	}
	return 0;
}


// helper function to reconstruct the hufftree
HCNode* restruct_tree(HCNode* root, queue< pair<byte, int> >& leaf_queue)
{
  HCNode* temp, *last = root;
  int templevel = 1;  // current level
  int needbreak = 0;  // when set the leaf, set needbreak to 1
  byte leaf_node;
  int leaf_level;
  while ( !leaf_queue.empty() ) // queue not empty
  {
    leaf_node = leaf_queue.front().first;  // get leaf
    leaf_level = leaf_queue.front().second;
    leaf_queue.pop();
    needbreak = 0;  // clear break condition
    do {
      if (last->c0 == 0 || last->c1 == 0) {  // parent node has less than 2 children
         if (templevel < leaf_level) temp = new HCNode(templevel,0); // intermediate node
         else if (templevel == leaf_level) {     // leaf node
            temp = new HCNode(leaf_level, leaf_node);
            needbreak = 1;
         }
      }
      if (last->c0 == 0) {  // left child
         temp->p = last;
         last->c0 = temp;
         templevel++;
         last = temp;
      }
      else if (last->c1 == 0) {  // right child
         temp->p = last;
         last->c1 = temp;
         templevel++;
         last = temp;
      }
      else {       // back search parent node
         templevel--;
         last = last->p;
      }
    }while(templevel <= leaf_level || needbreak == 0);

    templevel = leaf_level; // keep last leaf level
    last = last->p;  // keep last leaf's parent node
  }
    return root;
}
