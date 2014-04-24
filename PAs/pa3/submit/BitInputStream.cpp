// Yue Fan <yuf011@ucsd.edu>, Chaoqun Zhu <c9zhu@ucsd.edu>
#include "BitInputStream.hpp"

/** write to buffer
 * TESTED!*/
int
BitInputStream::readBit(){
	// bit buffer is empty
	if ( bufi == 0 ){
		buf = in.get();
		if ( in.eof() ) return -1;
		bufi = 8;
	}
	bufi --;
	int tmp = (buf >> bufi) & 0x01;//the leftmost 0 guarantees it's positive after sign ex
	return tmp;
}


/**read directly a byte to ostream*/
int
BitInputStream::readByte(){
	int byte2int = in.get();    // type cast
	if (in.eof()) return -1;
	byte2int = byte2int & 0x000000FF;
	return byte2int;
}

/**read a 32-bit value from input stream*/
int
BitInputStream::readInt(){
	int int_buffer = 0;
	// read 4 times from the input strea, do sanity check every time
	for ( int i = 0; i < 4; i++){
		int tmp_char = in.get() & 0x000000FF;
		if ( in.eof() ) return -1;
		int_buffer = (int_buffer << 8) | tmp_char; 	
	}
	return int_buffer;
}
