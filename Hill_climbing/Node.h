#ifndef __NODE__
#define __NODE__

#include <list>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>

#define ALPHABET_SIZE 26

using namespace std;

class Dictionary_Node {

	private:
		Dictionary_Node* alphabet_nodes [ ALPHABET_SIZE ];
		Dictionary_Node* parent;
		int height;
		int min_depth;
		int max_depth;
		bool is_end_of_word;

	public:

		Dictionary_Node();


		Dictionary_Node ( Dictionary_Node *parent , int height , int min_depth , int max_depth );


		~Dictionary_Node();


		void add_alphabet ( char alphabet , int word_size );


		void get_new_word ( int word_size );


		Dictionary_Node* get_node ( char alphabet );


		Dictionary_Node* get_parent();


		int 	get_height();


		int 	get_min_depth();


		int 	get_max_depth();


		bool get_is_end_of_word();


		void set_parent ( Dictionary_Node* parent);


		void set_height ( int height);


		void set_min_depth ( int min_depth);


		void set_max_depth ( int max_depth);


		void set_is_end_of_word ( bool is_end_of_word);


		void set_all_alphabets_to_null();


		static int get_alphabet_index (char alphabet){

			if ( alphabet <= 'z' && alphabet >= 'a' )
				return alphabet - 'a';
			else if ( alphabet <= 'Z' && alphabet >= 'A' )
				return alphabet - 'A';
			else
				return -1;
		}
					
};




#endif
