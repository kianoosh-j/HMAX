#include "../headers/utility_functions.h"


void print_list ( list<string> &str_list ){
	list<string>::iterator iter;

	for ( iter = str_list.begin() ; iter != str_list.end() ; iter++ ){
		cout<<"\t"<<(*iter)<<endl;
	}
}

