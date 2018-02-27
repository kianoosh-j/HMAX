#include "headers.h"
#include "C1.h"
#include "image_processing.h"
#include "picture_handler.h"

#define ARGS 2


int main( int argn , char *argv[]){


	if ( argn < ARGS ){
		cerr<<"number of arguments should be "<<ARGS<<endl;
		return -1;
	}

	int arg_index = 1;
	string file_name = argv[arg_index++];

	Picture test_picture;
	list<list<Picture> > S1_Pictures;
	list<list<Picture> > C1_Pictures;
	list<list<Picture> > gabor_filters;
       	vector<int> c1SpaceSS,c1ScaleSS;
	int c10L;

	init_parameters_for_C1 ( gabor_filters , c1SpaceSS , c1ScaleSS , c10L );

	cout<<"gabor filters\n";
	print_list_list_picture ( gabor_filters );

	read_picture_from_file ( test_picture , file_name );
	print_picture ( test_picture );

	C1_Function ( test_picture , S1_Pictures , C1_Pictures , gabor_filters, c1SpaceSS, c1ScaleSS, c10L );

	cout<<"S1_pictures\n";
	print_list_list_picture ( S1_Pictures );

	cout<<"C1_pictures\n";
	print_list_list_picture ( C1_Pictures );

	return 0;

}
