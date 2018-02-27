#include "../headers/utility_functions.h"


void print_list ( list<string> &str_list ){
	list<string>::iterator iter;

	for ( iter = str_list.begin() ; iter != str_list.end() ; iter++ ){
		cout<<"\t"<<(*iter)<<endl;
	}
}

void match_number_to_patches(list<list<Picture> > &C1_Pictures, int number){
	for(list<list<Picture> >::iterator it = C1_Pictures.begin(); it != C1_Pictures.end(); it++)
			for(list<Picture>::iterator it2 = it->begin(); it2 != it->end(); it2++)
				it2->NumberInDirectory = number;


}
