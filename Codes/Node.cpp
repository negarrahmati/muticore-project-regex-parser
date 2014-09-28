#include "Node.h"


Dictionary_Node::Dictionary_Node(){

	set_parent(NULL);
	set_height(0);
	set_min_depth(0);
	set_max_depth(0);
	set_all_alphabets_to_null();
	set_is_end_of_word ( false );


}


Dictionary_Node::Dictionary_Node ( Dictionary_Node *parent , int height , int min_depth , int max_depth ){
	set_parent(parent);
	set_height(height);
	set_min_depth(min_depth);
	set_max_depth(max_depth);
	set_all_alphabets_to_null();
	set_is_end_of_word ( false );
}


Dictionary_Node::~Dictionary_Node(){

	for ( int i=0 ; i<ALPHABET_SIZE ; i++ )
		if ( alphabet_nodes[i] != NULL )
			alphabet_nodes[i]->~Dictionary_Node();
}


void Dictionary_Node::add_alphabet ( char alphabet , int word_size ){

	int alphabet_index = get_alphabet_index (alphabet);

	get_new_word ( word_size );

	if ( alphabet_nodes[ alphabet_index ] == NULL ){
		alphabet_nodes[ alphabet_index ] = new Dictionary_Node( this , height + 1 , word_size , word_size);
	}
	else{
		alphabet_nodes[ alphabet_index ]->get_new_word(word_size);
	}
}


void Dictionary_Node::get_new_word ( int word_size ){

	if ( min_depth == 0 ){
		min_depth = word_size;
	}
	set_min_depth ( min ( min_depth , word_size ) );
	set_max_depth ( max ( max_depth , word_size ) );
}


Dictionary_Node* Dictionary_Node::get_node ( char alphabet ){
	return alphabet_nodes [ get_alphabet_index(alphabet ) ] ;
}


Dictionary_Node*	Dictionary_Node::get_parent(){
	return parent;
}


int 	Dictionary_Node::get_height(){
	return height;
}


int 	Dictionary_Node::get_min_depth(){
	return min_depth;
}


int 	Dictionary_Node::get_max_depth(){
	return max_depth;
}


bool Dictionary_Node::get_is_end_of_word(){
	return is_end_of_word;
}


void Dictionary_Node::set_parent ( Dictionary_Node* parent){
	this->parent = parent;
}


void Dictionary_Node::set_height ( int height){
	this->height = height;
}


void Dictionary_Node::set_min_depth ( int min_depth){
	this->min_depth = min_depth;
}


void Dictionary_Node::set_max_depth ( int max_depth){
	this->max_depth = max_depth;
}


void Dictionary_Node::set_is_end_of_word ( bool is_end_of_word){
	this->is_end_of_word = is_end_of_word;
}


void Dictionary_Node::set_all_alphabets_to_null(){
	for ( int i=0 ; i<ALPHABET_SIZE ; i++ ){
		alphabet_nodes[i] = NULL;
	}
}


