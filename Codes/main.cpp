#include "Dictionary.h"
#include <iostream>
#include <fstream>
#include <list>
#include <string>

#define MAX 20
#define ALPHABET_SIZE 26

char crossword[MAX][MAX];
int black_pos[MAX][MAX];
int black_pos_column[MAX][MAX];
int probability[MAX][MAX][ALPHABET_SIZE];

int prob_max;

using namespace std;

void print_table(int w,int h){
	for (int i = 0; i<w; i++){
		for(int j = 0; j<h ; j++){
			cout<<crossword[i][j]<<" ";
		}
		cout<<endl;
	}
}

int find_min_row(string s,int row, int f_column){
	int prob_min = 100000;
	for(int i = 0; i<s.size(); i++){
		if(probability[row][f_column+i][s[i]-'a']<prob_min)
			prob_min = probability[row][f_column+i][s[i]-'a'];
	}
	return prob_min;
}

void full_random_rows(Dictionary& dic,int w,int h){
	for(int i = 0; i<w ; i++){
		int counter = 0;
		int first = 0;
		while(black_pos[i][counter] != -1){
			int size = black_pos[i][counter]-first;
			//cout<<"i = "<<i<<" counter = "<<counter<<" size = "<<size<<endl;
			if(size > 1){
				string s = dic.get_random_word_by_size(size);
				if(s == ""){
					cout<<"we cannot full this table!"<<endl;
					exit(0);
				}
				else{
					//cout<<"first = "<<first<<endl;
					for(int j = first; j<first+size ; j++){
						crossword[i][j] = s[j-first];
					}
				}
			}
			first = black_pos[i][counter]+1;
			counter++;

		}
	}

}

void replace_row(Dictionary& dic, int w, int h,int size){
	vector<string> all_possible_words;
	vector<char> all_chars;
	all_chars.resize(ALPHABET_SIZE);
	for(int i = 0; i<ALPHABET_SIZE ; i++)
		all_chars[i] = i + 'a';

	dic.get_words_by_chars_position(all_possible_words,all_chars,size,0);

	int num = all_possible_words.size();
	cout<<"in size: "<<size<<" num: "<<num<<endl;

	for(int i = 0; i<w ; i++){
		int counter = 0;
		int first = 0;
		while(black_pos[i][counter]!= -1){
			int current_size = black_pos[i][counter]- first;
			if(current_size == size){
				int max_p = 0;
				int max_id;
				for(int j = 0; j<num ; j++){
					int p = find_min_row(all_possible_words[j],i,first);
					if(p>max_p){
						max_p = p;
						max_id = j;
					}
				}
				for(int k = 0; k<size ; k++){
					crossword[i][first+k] = all_possible_words[max_id][k];
				}
			}
			first = black_pos[i][counter]+1;
			counter++;
			
		}
	}
}

void find_probability(Dictionary& dic,int w, int h){
	int black_counter_column[MAX];
	int first_column[MAX];

	prob_max = 0;
	for(int i = 0; i<h ; i++){
		black_counter_column[i] = 0;
		first_column[i] = 0;
	}
	for(int i = 0; i < w ; i++){
		int black_counter_row = 0;
		int first_row = 0;
		for(int j = 0; j<h ; j++){
			if(crossword[i][j] != 'X'){
				for(int k = 0; k<ALPHABET_SIZE; k++){
					int row_num, column_num;
					int row_size = black_pos[i][black_counter_row] - first_row;
					int row_pos = j - first_row;
					if(row_size>1){
						row_num = dic.size_index[row_size-2].positions[row_pos].alphabet_indexs[k+1] - dic.size_index[row_size-2].positions[row_pos].alphabet_indexs[k];
						//if(row_num == 0)
						//	cerr<<"row_size: "<<row_size<<" row_pos: "<<row_pos<<endl;
					}

					int column_size = black_pos_column[j][black_counter_column[j]] - first_column[j];
					int column_pos = i - first_column[j];
					if(column_size>1)
						column_num = dic.size_index[column_size-2].positions[column_pos].alphabet_indexs[k+1] - dic.size_index[column_size-2].positions[column_pos].alphabet_indexs[k];
					if(row_size>1 && column_size>1)
						probability[i][j][k] = row_num<column_num? row_num : column_num;
					else if(row_size>1)
						probability[i][j][k] = row_num;
					else if(column_size>1)
						probability[i][j][k] = column_num;
					else{
						probability[i][j][k] = 0;
						cerr<<"row_size: "<<row_size<<" column_size: "<<column_size<<endl;
					}
					if(probability[i][j][k]>prob_max)
						prob_max = probability[i][j][k];
				}
			}
			else{
				black_counter_row++;
				first_row = j+1;
				black_counter_column[j]++;
				first_column[j] = i+1;
			}

		}
	}
}

int find_correct_column(Dictionary& dic, int w, int h){
	int num = 0;
	for(int i= 0; i<h ; i++){
		int counter = 0;
		int first = 0;
		while(black_pos_column[i][counter]!= -1){
			int size = black_pos_column[i][counter] - first;
			string s;
			s.resize(size);
			for(int j = 0; j<size ; j++){
				s[j] = crossword[first+j][i];
			}
			if(dic.search_word(s)){
				cout<<"correct word in row: "<<first<<" and column: "<<i<<" is: "<<s<<endl;
				num++;
			}

			first = black_pos_column[i][counter] + 1;
			counter++;
		}
	}
	return num;
}

int main(){
	ifstream inFile;
	string inStr;
	list <string> words;
	inFile.open("../word_list_moby_crossword.flat.txt",ifstream::in);
	if(!inFile){
		cerr<<"file did not open!"<<endl;
		exit(-1);
	}
	while(inFile>>inStr){
		words.push_back(inStr);
	}

	Dictionary dic(words);
	dic.create_index();
//....................................................................test	
	//vector<string> a41;
	//vector<string> b22;
	//vector<string> z108;

	//a41 = dic.get_words_by_char_position('a',4,1);
	//int sb22 = dic.get_words_by_char_position(b22,'b',4,2);
	//z108 = dic.get_words_by_char_position('z',10,8);
	//cout<<dic.size_index[2].positions[2].alphabet_indexs['b'-'a']-dic.size_index[2].positions[2].alphabet_indexs['c'-'a']<<endl;
	//cout<<b22.size()<<endl;
	//cout<<sb22<<endl;
	//cout<<z108.size()<<endl;
//......................................................................test
	int n,m;
	cin>>n>>m;
	//char* crossword;
	//crossword = new char[n*m];
	for(int i = 0; i<n; i++){
		int counter = 0;
		for (int j = 0; j<m ; j++){
			cin>>crossword[i][j];
			if(crossword[i][j] == 'X'){
				black_pos[i][counter++] = j;
			}
			//counter++;
		}
		black_pos[i][counter++] = m;
		black_pos[i][counter++] = -1;
	}

	for(int i = 0; i<m ; i++){
		int counter = 0; 
		for(int j = 0; j<n ; j++){
			if(crossword[j][i] == 'X'){
				black_pos_column[i][counter++]= j;
			}
		}
		black_pos_column[i][counter++] = n;
		black_pos_column[i][counter++] = -1;
	}

	/*for(int i = 0; i<n ; i++){
		int counter = 0;
		while(black_pos[i][counter] != -1){
			cout<<black_pos[i][counter]<<" ";
			counter++;
		}
		cout<<endl;
	}*/
	//cout<<"................"<<endl;
	dic.create_dictionary_tree();
	full_random_rows(dic,n,m);
	print_table(n,m);
	cout<<"corrects: "<<find_correct_column(dic,n,m)<<endl;

	find_probability(dic,n,m);
	/*for(int i = 0; i<n; i++){
		for(int j = 0; j<m; j++){
			cout<<"column: "<<j<<":: ";
			if(crossword[i][j]!='X'){
				for(int k = 0; k<26 ; k++){
					cout<< k<< ": "<<probability[i][j][k]<<" ";
				}
			}
			else{
				cout<<"black ";
			}
			cout<<"| ";
		}

		cout<<"\n";
	}*/
	cout<<".............................\n";
	//cout << prob_max<<endl;
	replace_row(dic,n,m,4);
	print_table(n,m);

	int num_correct_column = find_correct_column(dic,n,m);
	cout<<"corrects: "<<num_correct_column<<endl;

	cout<<"............................\n";
	replace_row(dic,n,m,3);
	print_table(n,m);

	cout<<"correct: "<<find_correct_column(dic,n,m)<<endl;

	cout<<"............................\n";
	replace_row(dic,n,m,8);
	print_table(n,m);

	cout<<"correct: "<<find_correct_column(dic,n,m)<<endl;

	cout<<"............................\n";
	replace_row(dic,n,m,9);
	print_table(n,m);

	cout<<"correct: "<<find_correct_column(dic,n,m)<<endl;

	cout<<"............................\n";
	replace_row(dic,n,m,6);
	print_table(n,m);

	cout<<"correct: "<<find_correct_column(dic,n,m)<<endl;

	cout<<"............................\n";
	replace_row(dic,n,m,5);
	print_table(n,m);

	cout<<"correct: "<<find_correct_column(dic,n,m)<<endl;


	return 0;
}
