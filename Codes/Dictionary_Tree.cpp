#include "Dictionary_Tree.h"



Dictionary_Tree::Dictionary_Tree(){
	parent_node = new Dictionary_Node();
}


Dictionary_Tree::~Dictionary_Tree(){
	parent_node->~Dictionary_Node();
}


void Dictionary_Tree::add_word(string word){

	// check if parent node is null
	if ( parent_node == NULL ){
		parent_node = new Dictionary_Node();
	}

	// get the current node ; it moves over tree nodes
	Dictionary_Node *current_node = parent_node;
	current_node->set_min_depth(0);

	for ( int i=0 ; i<word.length() ; i++ ){

		if ( current_node == NULL ){
			exit(0);
		}
		current_node->add_alphabet ( word[i] , word.length() );
		current_node = current_node->get_node ( word[i] );
	}
	current_node->set_is_end_of_word ( true );
}


vector<char> Dictionary_Tree::get_possible_chars ( string str , int max_remaining_alphabets ){

	vector<char> possible_chars;
	Dictionary_Node *current_node = parent_node;
	if ( current_node == NULL )
		return possible_chars;

	for ( int i=0 ; i<str.length() ; i++ ){

		if ( current_node == NULL ){
			return possible_chars;
		}
		current_node = current_node->get_node ( str[i] );

	}

	if ( current_node != NULL ){
		for ( char alphabet = 'a' ; alphabet <= 'z' ; alphabet++  ){

			Dictionary_Node *alphabet_node  = current_node->get_node ( alphabet );
			if ( alphabet_node != NULL ){
				if ( max_remaining_alphabets == -1 || 
						(max_remaining_alphabets > 0 
						 && (alphabet_node->get_min_depth() - alphabet_node->get_height()) < max_remaining_alphabets )){

					possible_chars.push_back ( alphabet );
				}
			}

		}
	}

}


bool Dictionary_Tree::search_word ( string word ){

	Dictionary_Node *current_node = parent_node;
	for ( int i=0  ; i<word.length() ; i++ ){

		if ( current_node != NULL )
			current_node = current_node->get_node ( word[i] );

	}
	if ( current_node != NULL ){
		return current_node->get_is_end_of_word ();
	}
	return false;

}


