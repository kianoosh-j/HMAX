#include "../headers/errors.h"

#include <iostream>

using namespace std;

void show_error ( string file_name , string method_name , string error_message ){

	cerr << "Error in file : " << file_name << "\n\t\t in method : " << method_name << "\n\t\t" << " error message : " << error_message<<endl;
}
