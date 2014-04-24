// Yue Fan <yuf011@ucsd.edu>, Chaoqun Zhu <c9zhu@ucsd.edu>
#include "HCTree.hpp"

/**constructor already defined
 * need only to define the destructor*/
HCTree::~HCTree(){
	postOrder(root);
}


// modifier function
void
HCTree::setroot(HCNode* rootin){
	root = rootin;
}

// helper function for deleteing the entire tree
void
HCTree::postOrder(HCNode* curr){
	// for a huffman tree, the following holds:
	// if a node has at least one child,
	// it must be the case that it has both left and right
	if (curr->c0 != 0 && curr->c1 != 0){
		postOrder(curr->c0);
		postOrder(curr->c1);
		delete curr;
	}
	else if (curr->c0 != 0 ){
		postOrder(curr->c0);
	}
	// if it's a leaf
	else{
		delete curr;
	}
}

/**build the Huffman tree*/
void
HCTree::build(const vector<int>& freqs){

	// first get the data structure we're gonna need
	std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> nodeForest;

	// construct a priority queue with this freqs vector
	for ( int i = 0; i < freqs.size(); i++){
		if (freqs[i] != 0){
			HCNode* tmp = new HCNode(freqs[i], (unsigned char)i);
			nodeForest.push(tmp); // when we're pushing, a copy of the item being pushed is stored
		}
	}

	// here we are assuming that there are more than 1 node in the forest
	// but it's possible that we have only one symbol in a file as a corner case
	// we don't change the size here
	if ( nodeForest.size() == 1 ){
		HCNode* tmp_node = nodeForest.top();

		// make a new root
		root = new HCNode(0,0);

		root->c0  = tmp_node;
		tmp_node->p = root;
	}

	else{
		// build the Huffman tree!
		while (nodeForest.size() != 1){
			//take the most prioritized 2 nodes out
			HCNode* tmp0 = nodeForest.top();	
			nodeForest.pop();

			HCNode* tmp1 = nodeForest.top();
			nodeForest.pop();

			byte tmp_symbol = 0;

			// construct their parent
			HCNode* tmp_p = new HCNode( tmp0->count + tmp1->count,tmp_symbol );

			// link the parent with their children
			// since tmp0 is the first one popped out, tmp0 is always bigger
			// so c0, the left child is always bigger
			tmp_p ->c0 = tmp0;
			tmp_p ->c1 = tmp1;
			tmp0  ->p  = tmp_p;
			tmp1  ->p  = tmp_p;
			tmp_p ->symbol = (tmp0->symbol - tmp1->symbol <0)? tmp0->symbol : tmp1->symbol;

			// push the new node back into queue
			nodeForest.push(tmp_p);
		}

		/** root point to the top of HTree
		 */
		this->root = nodeForest.top();	
	}

	leafFinder(this->root);
}

/**encode function*/
void
HCTree::encode(byte symbol, BitOutputStream &out) const{
	int index = int(symbol) & 0xFF;
	HCNode* ptr_symbol = leaves[index];
	recursive_write(ptr_symbol, out);
}


int
HCTree::decode(BitInputStream& in) const{
	return 1;
}

//helper function: write the encoding bit-by-bit
void
HCTree::recursive_write(HCNode* curr, BitOutputStream &out) const{
	if(curr->p != 0){
		recursive_write(curr->p, out);
		if (curr->p->c0 == curr)
					out.writeBit(0); // left -> write 0 
		else 	out.writeBit(1); // right -> write 1	
	}
  else return;
}

//** find all the leaves and put them into the 256-element vector*/
void
HCTree::leafFinder(HCNode* curr){
	if (curr ->c0 != 0 && curr ->c1 != 0){
		global_level++;
		leafFinder(curr->c0);
		global_level--;

		global_level++;
		leafFinder(curr->c1);
		global_level--;
	}
	else if (curr ->c0 != 0){
		global_level++;
		leafFinder(curr->c0);
		global_level--;
	}
	else if (curr ->c1 != 0){
		global_level++;
		leafFinder(curr->c1);
		global_level--;
	}
	else{
		/** which means current node neither has right nor left child --> it's a leaf!
		 */
		int index = curr ->symbol;
		index = index & 0xFF;
		leaves[index] = curr;

		// construct a queue of leaves from left to right
		// save us another traversal afterwards!
		this->lh2rh.push( std::make_pair(char(index), global_level) );
	}
}

/** find all the leaves and put them into the 256-element vector*/
void
HCTree::leafTraverse_public( queue< pair<byte, int> >& leaf_queue ){
	// assign one queue to another
	leaf_queue = this->lh2rh;
}
