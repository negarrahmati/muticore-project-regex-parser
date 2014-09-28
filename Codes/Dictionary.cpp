#include "Dictionary.h"

#include <iterator>
#include <algorithm>

int compare_position;


bool compare_string_by_position(string a , string b){

	if ( a.length() > compare_position && b.length() > compare_position ){
		return a[compare_position] < b[compare_position];
	}
	return true;
}


bool compare_string_size ( string str1 , string str2 ){
	return str1.length() < str2.length();
}


Dictionary::Dictionary(){
	min_word_length = 0;
	max_word_length = 0;
}


Dictionary::Dictionary(list<string> words){

	set_words(words);
}


Dictionary::~Dictionary(){

}


void Dictionary::create_dictionary_tree(){

	for ( vector<string>::iterator iter = words.begin() ; iter != words.end() ; iter++){
		dictionary_tree.add_word ( *iter);
	}

}


void Dictionary::create_index(){

	if ( word_id_map.size() == 0 )
		create_word_id_map();

	// create bags of words for each size
	vector<string> word_bags[MAX_WORD_LENGTH - MIN_WORD_LENGTH + 1];// each list contain words of specific length
	for ( vector<string>::iterator iter = words.begin() ; iter != words.end() ; iter++ ){
		word_bags[iter->length() - MIN_WORD_LENGTH ].push_back ( *iter );
	}

	// create word indexes array
	total_word_size_position = 0;
	for ( int i=0 ; i< (MAX_WORD_LENGTH-MIN_WORD_LENGTH+1) ; i++ ){
		total_word_size_position += word_bags[i].size() * (i + MIN_WORD_LENGTH );
	}
	word_indexes = new int [total_word_size_position];

	// for each word size create the position index
	int base_index = 0; // it is index over word_indexes array
	for ( int i=0 ; i< (MAX_WORD_LENGTH-MIN_WORD_LENGTH+1) ; i++ ){
		
		if ( word_bags[i].size() > 0 ){
			// for each position
			int words_size = i + MIN_WORD_LENGTH ;
			for ( int pos = 0 ; pos < words_size ; pos++ ){

				// sort all words according to position
				sort_words_according_to_position ( word_bags[i] , pos );

				// put sorted ids in the word_indexes array
				for ( vector<string>::iterator iter= word_bags[i].begin() ; iter != word_bags[i].end() ; iter++ ){

					map<string,int>::iterator find_element_iter = word_id_map.find(*iter);
					if ( find_element_iter != word_id_map.end() ){
						word_indexes[base_index++] = find_element_iter->second;
					}
					else{
						cerr<<"error : cannot find the word in the map\n";
						exit(0);
					}

				}
				base_index -= word_bags[i].size();

				// create indexes
				char current_char = 'a';
				int current_index = 0;
				size_index[i].positions[pos].alphabet_indexs[current_index] = base_index;
				current_index++;
				for ( vector<string>::iterator iter= word_bags[i].begin() ; iter != word_bags[i].end() ; iter++ , base_index++ ){

					// if the current word char at pos differs from current char
					while ( current_char <= 'z' && (*iter)[pos] != current_char ){
						size_index[i].positions[pos].alphabet_indexs[current_index] = base_index;
						current_char++;
						current_index++;
					}

				}
				while ( current_char <= 'z' ){
					size_index[i].positions[pos].alphabet_indexs[current_index] = base_index;
					current_char++;
					current_index++;
				}
				

			}
		}
	}

}


void Dictionary::create_word_id_map(){

	int id = 0;
	word_id_map.clear();
	for ( vector<string>::iterator iter = words.begin() ; iter != words.end() ; iter++ , id++ ){
		word_id_map.insert ( pair<string,int>(*iter , id) );
	}
}


void Dictionary::sort_words_according_to_position(vector<string> &words , int pos ){

	compare_position = pos;
	sort ( words.begin() , words.end() , compare_string_by_position );
}


void Dictionary::set_words(list<string> words){

	this->words.clear();
	this->words.insert ( this->words.end() , words.begin() , words.end() );
	for ( vector<string>::iterator iter = this->words.begin() ; iter != this->words.end() ; iter++  ){
		word_sizes.insert ( iter->length() );
	}
	sort_words_according_to_position ( this->words , 0 );
	set_id_to_string();
	min_word_length = min_element (words.begin() , words.end()  , compare_string_size )->length();
	max_word_length = max_element (words.begin() , words.end()  , compare_string_size )->length();
	cerr<<"min word length is "<<min_word_length<<"and max is "<<max_word_length<<endl;
}


void Dictionary::print_words(){

	int id = 0;
	for ( vector<string>::iterator iter = words.begin() ; iter != words.end() ; iter++ , id++ )
		cout<<"\t"<<id<<" "<<*iter<<endl;
	cout<<endl;
	//for ( int i=0 ; i<total_word_size_position ;i++ )
	//	cout<<"\t"<<word_indexes[i]<<endl;
}


void Dictionary::set_id_to_string(){

	id_to_string.clear();
	sort_words_according_to_position (words , 0 );
	
	int id = 0;
	for (vector<string>::iterator iter = words.begin() ; iter != words.end() ; iter++ , id++){
		id_to_string.insert ( pair<int , string>(id , *iter) );
	}

}


string Dictionary::get_random_word_by_size(int size){
	
	int startding_index = size_index[size-MIN_WORD_LENGTH].positions[0].alphabet_indexs[0];
	int ending_index = size_index[size-MIN_WORD_LENGTH].positions[0].alphabet_indexs[ALPHABET_SIZE];


	if ( startding_index == ending_index ){
		cerr<<"no word with size "<<size<<"\texists in dictionary\n";
		return "";
	}
	int rand_index = (rand() % (ending_index - startding_index)) + startding_index ;
	map<int , string>::iterator map_iter = id_to_string.find ( word_indexes [ rand_index ] );
	if ( map_iter != id_to_string.end() ){
		return map_iter->second;
	}
	else{
		cerr<<"error in get_random_word_by_size : illegal id\n";
		exit(0);
	}
	

}


vector<char> Dictionary::get_possible_chars ( string str , int max_remaining_alphabets ){

	return dictionary_tree.get_possible_chars ( str , max_remaining_alphabets );
}


void Dictionary::get_words_by_chars_position ( vector<string> &all_possible_words , vector<char> &possible_chars , int word_size , int pos ){

	for ( vector<char>::iterator iter = possible_chars.begin() ; iter != possible_chars.end() ; iter++ ){

		vector<string> words;
		get_words_by_char_position ( words ,*iter , word_size , pos );
		cerr<<" found words for this alphabet is "<<words.size()<<endl;
		all_possible_words.insert ( all_possible_words.end() , words.begin() , words.end() );
		cerr<<"all_possible_words size is "<<all_possible_words.size()<<endl;
		words.clear();
		
	}
	cerr<<"salam\n";

}


int Dictionary::get_words_by_char_position ( vector<string>& all_words, char alphabet , int word_size , int position ){

	int alphabet_index = Dictionary_Node::get_alphabet_index ( alphabet );

	int lower_bound = size_index[word_size-MIN_WORD_LENGTH].positions[position].alphabet_indexs[alphabet_index];
	int upper_bound = size_index[word_size-MIN_WORD_LENGTH].positions[position].alphabet_indexs[alphabet_index+1];

	all_words.resize(upper_bound-lower_bound);
	for ( int i=lower_bound ; i<upper_bound ; i++ ){

		if ( id_to_string.find ( word_indexes[i] ) != id_to_string.end() )
			all_words[i-lower_bound] = id_to_string.find ( word_indexes[i] )->second;
		else{
			cerr<<"illegal word id\n";
			exit(0);
		}
	}
	return upper_bound - lower_bound;


}


bool 	Dictionary::search_word ( string word ){
	return dictionary_tree.search_word ( word );
}


bool Dictionary::has_word_size(int size){

	return word_sizes.find ( size ) != word_sizes.end();

}

