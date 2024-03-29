#include "BST.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/**
 * A simple test driver for the BST class template.
 * P1 CSE 100 2013
 * Author: P. Kube (c) 2013
 */
int main() {

  /* Create an STL vector of some ints */
  vector<int> v;
  v.push_back(3);
  v.push_back(4);
  v.push_back(1);
  v.push_back(100);
  v.push_back(-1);
  v.push_back(0);
  v.push_back(2);
  v.push_back(-5);

  /* Create an instance of BST holding int */
  BST<int> b;

  /* Insert a few data items. */
  vector<int>::iterator vit = v.begin();
  vector<int>::iterator ven = v.end();

  for(; vit != ven; ++vit) {
    // all these inserts are unique, so should return a std::pair
    // with second part true
    std::pair<BST<int>::iterator,bool> pr = b.insert(*vit);
    if(! pr.second ) {
      cout << "Incorrect bool return value when inserting " << *vit << endl;
      return -1;
    }
    if(*(pr.first) != *vit) {
      cout << "Incorrect iterator return value when inserting " << *vit << endl;
      return -1;
    }  
    cout << "+1" << endl;
  }

//  /* Test size. */
  cout << "Size is: " << b.size() << endl;
  if(b.size() != v.size()) {
    cout << "... which is incorrect." << endl;
    return -1;
  }
//
  /* Test find return value. */
  vit = v.begin();
  for(; vit != ven; ++vit) {
    if(*(b.find(*vit)) == *vit) {
      cout << "correct return value when finding " << *vit << endl;
    //  return -1;
    }
  }

// the following snipet is used to test: finding a value that is not in the BST
//  if (*(b.find(34)) == 100)
//	cout<< 12344 << endl; 
	
  
//  /* Sort the vector, to compare with inorder iteration on the BST */
  sort(v.begin(),v.end());

  /* Test BST iterator; should iterate inorder */
  cout << "traversal using iterator:" << endl;
  vit = v.begin();

  BST<int>::iterator en = b.end();
  BST<int>::iterator it = b.begin();

	cout<< "begin of bst is:" << *it << endl;
//	cout<< "end of bst is:" << en << endl;

  for(; vit != ven; ++vit) {
    if(! (it != en) ) {
      cout << *it << "," << *vit << ": Early termination of BST iteration." << endl;
      return -1;
    }
    cout << *it << endl;
    if(*it != *vit) {
      cout << *it << "," << *vit << ": Incorrect inorder iteration of BST." << endl;
      return -1;
    }
//	cout<<"before\n"<<endl;
    ++it;
//	cout<< *it<<endl;
//	cout<<"here\n"<<endl;
  }
  cout << "OK." << endl;
	
  //test clear
  //BST<int>* b_ptr = &b;
  //delete b_ptr;

}
