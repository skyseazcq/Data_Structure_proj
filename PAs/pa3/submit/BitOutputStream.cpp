// Yue Fan <yuf011@ucsd.edu>, Chaoqun Zhu <c9zhu@ucsd.edu>
#include "BitOutputStream.hpp"

// definition of functions
/** write to buffer*/
void
BitOutputStream::writeBit(int bit){
	
	// if the buffer is full, flush!
	if ( bufi == 0 ){
		flush(); // flush will automatically return the buffer to its initial state
	}

	// write the least sig fig of bit to buffer
	if ( bit == 1 ){
		char mask = 1 << (bufi - 1);
		buf = buf | mask;
	} 		
	else if ( bit == 0 ){
		// leave everything as it is;
	} 
	else{
		cout << "bad bit number!!" << endl;
	}
	bufi --; //incease the index for next bit write
}


/**write directly a byte to ostream
 */

void
BitOutputStream::writeByte( int b ){
	//char trunc_in = (char) b;
	//out.put(trunc_in);
	out.put(char(b));
}

/**writing a 32-bit value into output stream
 * */
void
BitOutputStream::writeInt( int i ){
	int tmp_in = 0;
	for (int k = 3 ; k >= 0  ; k--){
		//tmp_in = ( i >> ( k * sizeof(char)) ) & 0x000000FF;
		tmp_in = ( i >> ( k << 3) ) & 0x000000FF;
		out.put( char(tmp_in) ); // new a char, with typt casting
	}
}

/**flushing*/
void
BitOutputStream::flush(){
	if ( bufi != 8 ){
		out.put(buf);
		buf = 0; // set all the element to 0
		bufi = 8;    // return the index to the beginning
		out.flush(); //here we can see the diff between delegation and inheritance
	}
}

