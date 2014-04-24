#ifndef BST_HPP
#define BST_HPP
#include "BSTNode.hpp"
#include "BSTIterator.hpp"
#include <utility> // for std::pair


// Members:
// Chaoqun Zhu: A53026524
// Yue Fan:	A53031252


//jason:
//added this to use cout function to print out which node has been deleted
//using namespace std;

template<typename Data>
class BST {

protected:

  /** Pointer to the root of this BST, or nullptr if the BST is empty */
  BSTNode<Data>* root;

  /** Number of Data items stored in this BST. */
  unsigned int isize;

private:

// the functions defined here are overloaded functions, with different signatures
  std::pair<BSTIterator<Data>,bool> insert(const Data& item, BSTNode<Data>* & t, BSTNode<Data>* parent) {
	if ( t == nullptr ){
	        t = new BSTNode<Data>(item);// parent node already points here
		t->parent = parent;
		isize++;
		BSTIterator<Data> itr(t);
	        return make_pair(itr,true);// return type of make_pair is pair!returning by value
	}
	else if ( item < t->data ){
	        return insert(item, t->left, t);
	}
	else if ( item > t->data ){
	        return insert(item, t->right,t);
	}
	else{
		BSTIterator<Data> itr(t);
	        return make_pair(itr,false);
	}
  }

  BSTIterator<Data> find(const Data& item, BSTNode<Data>* t) const{
  	if ( t == nullptr ){
  		return end();
  	}
  	else if ( item < t->data ){
		return find(item,t->left);
	}
	else if ( item > t->data ){
		return find(item, t->right);
	}
	else{
		return BSTIterator<Data>(t); 
  	}

  }

  void delNode(BSTNode<Data>* curr){

	if (curr == nullptr) return; // when the ptr is null, we would get an error when dereferencing it

	if( curr->left != nullptr)
		delNode(curr->left);
	if (curr->right != nullptr)
		delNode(curr->right);

//	cout<<"deleting:"<< curr->data << endl;
	delete curr;
	curr = nullptr;
  }

public:

  /** iterator is an aliased typename for BSTIterator<Data>. */
  //jason's comment: should we put it out of this template so that every section(private,public and protected can all use it?)
  typedef BSTIterator<Data> iterator;

  /** Default constructor.
      Initialize an empty BST.
   */
  BST() : root(nullptr), isize(0)  {
  }


  /** Default destructor.
   *  It is virtual, to allow appropriate destruction of subclass objects.
   *  Delete every node in this BST.
   */ // TODO
  virtual ~BST() {
	clear();
  }

  /** Insert a Data item in the BST.
   *  Return a pair, with the pair's first member set to an
   *  iterator pointing to either the newly inserted element
   *  or to the equivalent element already in the set.
   *  The pair's second element is set to true
   *  if a new element was inserted or false if an
   *  equivalent element already existed.
   */ // TODO
  virtual std::pair<iterator,bool> insert(const Data& item) {
	return insert(item, root, nullptr);
  }


  /** Find a Data item in the BST.
   *  Return an iterator pointing to the item, or the end
   *  iterator if the item is not in the BST.
   */ // TODO
  iterator find(const Data& item) const {
  	return find(item,root);
  }

  /** Return the number of items currently in the BST.
   */ // TODO
  unsigned int size() const {
  	return isize;
  }

  /** Remove all elements from this BST, and destroy them,
   *  leaving this BST with a size of 0.
   */ // TODO
  void clear() {
	delNode(root);
  }

	
  /** Return true if the BST is empty, else false.
   */ // TODO
  bool empty() const {
  	//return (isize == 0);	
	if (isize == 0) return true;
	else 		return false;
  }

  /** Return an iterator pointing to the first item in the BST.
   */ // TODO
  iterator begin() const {
  	BSTNode<Data>* tmp;
  	tmp =  root;
  	while( tmp -> left != nullptr)	tmp = tmp->left;
  	return iterator(tmp);
  }

  /** Return an iterator pointing past the last item in the BST.
   */
  iterator end() const {
    return typename BST<Data>::iterator(nullptr);
  }

 };


#endif //BST_HPP
