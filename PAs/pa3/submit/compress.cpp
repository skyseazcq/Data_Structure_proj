// Yue Fan <yuf011@ucsd.edu>, Chaoqun Zhu <c9zhu@ucsd.edu>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCTree.hpp"
#include "HCNode.hpp"
#include <fstream>
#include <queue>

using namespace std;

int main(int argc, char *argv[]){
	// argv[0]--> prog name
	// argv[1]--> ptr to char array of infile
	// arge[2]--> ptr to char array of outfile
	if ( argc < 3 ){
		cout << "Wrong format!! USE: ./compress infile outfile" << endl;
		return 0;
	}

	/**use readByte to read and construct freqs vector*/
	ifstream sourceFile;
	ofstream targetFile;

	sourceFile.open(argv[1], ios::binary);
	//targetFile.open("./output.txt", ios::binary);

	// check if open fails
	if ( !sourceFile.good() ){
		cout << "failed to open file!" << cout;
		return 0;
	}
	

	BitInputStream source0(sourceFile);
	
	/**readByte of source returns -1 if eof*/
	vector<int> freqs(256,0);
	int byteIn = source0.readByte();
	int totalByte = 0; 				// number of total bytes, put it in compressed file for decoding
	int diffChar = 0; 				// number of distinct chars in the input file

	/**first test: whether empty file*/
	if (byteIn == -1){
		cout << "FILE IS EMPTY!" << endl;
		return 1;
	}

	while ( byteIn != -1){
		if (freqs[byteIn] == 0) diffChar ++;
		freqs[byteIn] ++;
		totalByte ++;

		byteIn = source0.readByte();
	}
	sourceFile.close();

	/**
	if (totalByte <= 1024 * 10){
		cout << "File size <= 10Kb! No need to compress." << endl;
		cout << "Terminating..." << endl;
		return 0;
	}
	*/

	/**build the tree*/
	HCTree huffTree;
	huffTree.build(freqs);

	// get input ready	
	sourceFile.open(argv[1], ios::binary);
	BitInputStream source(sourceFile);

	// get output ready
	targetFile.open(argv[2], ios::binary);
	BitOutputStream target(targetFile);
		
	// get the queue, left to right tree traversal
	queue< pair<byte,int> > leaf_queue;
	huffTree.leafTraverse_public(leaf_queue);

	// print the header to the file
	// 1. write total number of bytes
	target.writeInt(totalByte);

	// 2. write total unique char
	target.writeByte(diffChar);

	// 3. iteratively write (symbol, level)
	while ( !leaf_queue.empty()){
		int symbol = leaf_queue.front().first;
		int level  = leaf_queue.front().second;
		target.writeByte(symbol);
		target.writeByte(level);

		// pop the head out
		leaf_queue.pop();
	}

	byteIn = source.readByte();
	while (byteIn != -1){
		huffTree.encode(char(byteIn), target);
		byteIn = source.readByte();
	}

	// make sure to dump everything in there
	target.flush();

	// closing files
	targetFile.close();
	sourceFile.close();

	return 0;
}
