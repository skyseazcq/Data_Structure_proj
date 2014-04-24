// Yue Fan <yuf011@ucsd.edu>, Chaoqun Zhu <c9zhu@ucsd.edu>
#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include "HCNode.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */
/** This class must be defined to be used as the 3rd para to pri_queue*/
class HCNodePtrComp {
public:
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        return *lhs < *rhs;
    }
};

/** A Huffman Code Tree class.
 *  Not very generic:  Use only if alphabet consists
 *  of unsigned chars.
 */
class HCTree {
private:
  HCNode* root;
  vector<HCNode*> leaves;
	queue< pair<byte, int> > lh2rh;
	int global_level;

	void leafFinder(HCNode* curr);
	void recursive_write(HCNode* curr, BitOutputStream &out) const;
//	void leafTraverse( HCNode* curr, queue< pair<byte, int> >& leaf_queue );
	void postOrder(HCNode* curr);

public:

    explicit HCTree() : root(0) {
        leaves = vector<HCNode*>(256, (HCNode*) 0);
				global_level = 0;
    }

    ~HCTree();

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void build(const vector<int>& freqs);

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int decode(BitInputStream& in) const;
		
    // for the construction of queue, helper function	
    void leafTraverse_public( queue< pair<byte, int> >& leaf_queue );

    // modifier function
    void setroot(HCNode* rootin);
};

#endif // HCTREE_HPP
