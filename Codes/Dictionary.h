#ifndef __DICTIONARY__
#define __DICTIONARY__

#include <list>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <set>
#include <map>
#include "Dictionary_Tree.h"


#define ALPHABET_SIZE 26
#define MAX_WORD_LENGTH 30
#define MIN_WORD_LENGTH 2

using namespace std;


struct Position_Index {

	int alphabet_indexs[ALPHABET_SIZE+1];
};


struct Size_Index {

	Position_Index positions[MAX_WORD_LENGTH];
};


class Dictionary{

	public:
		// containing all words in a sorted way
		vector<string> words;
		map<int , string> id_to_string;
		int min_word_length;
		int max_word_length;
		set<int> word_sizes;

		// B-Tree data structure
		Dictionary_Tree dictionary_tree;

		// index data structure
		Size_Index size_index [MAX_WORD_LENGTH - MIN_WORD_LENGTH + 1];
		int *word_indexes;
		int total_word_size_position;
		map<string , int> word_id_map;

		void sort_words();
		void sort_words_according_to_position(vector<string> & , int pos );

	public:
		Dictionary();
		Dictionary(list<string> words);
		~Dictionary();
		void set_words(list<string> words);
		void set_id_to_string();
		void create_index();
		void create_dictionary_tree();
		void create_word_id_map();
		void print_words();
		string get_random_word_by_size(int size);
		// it returns possible chars that comes after this string and have word in dictionry for them
		vector<char> get_possible_chars ( string str , int max_remaining_alphabets=-1 );
		void get_words_by_chars_position ( vector<string> &all_possible_words , vector<char> &possible_chars , int word_size , int pos );
		int get_words_by_char_position (vector<string>& ,  char alphabet , int word_size , int position );
		bool search_word ( string word );
		bool has_word_size(int size);
		void set_min_word_length ( int min_word_length){
			this->min_word_length = min_word_length;
		}
		void set_max_word_length ( int max_word_length){
			this->max_word_length = max_word_length;
		}
		int get_min_word_length(){
			return min_word_length;
		}
		int get_max_word_length(){
			return max_word_length;
		}

};



#endif
