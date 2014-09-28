#ifndef __DICTIONARY__TREE__
#define __DICTIONARY__TREE__

#include <list>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <stdlib.h>

#include "Node.h"

using namespace std;

class Dictionary_Tree{

	private:
		Dictionary_Node *parent_node;

	public:

		Dictionary_Tree();


		~Dictionary_Tree();


		void add_word(string word);

		// it returns possible chars that comes after this string and have word in dictionry for them
		vector<char> get_possible_chars ( string str , int max_remaining_alphabets = -1 );

		bool search_word ( string word );


};


#endif
