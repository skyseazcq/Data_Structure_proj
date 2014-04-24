// Yue Fan <yuf011@ucsd.edu>, Chaoqun Zhu <c9zhu@ucsd.edu>
#include "HCNode.hpp"

/**constructor
 * NO RETURN TYPE FOR CONSTRUCTOR*/
HCNode::HCNode(int count, byte symbol, HCNode* c0, HCNode* c1, HCNode* p)
	: count(count), symbol(symbol), c0(c0), c1(c1), p(p) {}

/**less than operator: break ties using alphabetic order*/
bool
HCNode::operator<(const HCNode& other){
	if ( (*this).count != other.count )
		return (*this).count > other.count;
	else 
		return (*this).symbol > other.symbol;
}

/**comp is a friend function of HCNode*/
bool
comp(HCNode* one, HCNode* other){
	return true;

}
