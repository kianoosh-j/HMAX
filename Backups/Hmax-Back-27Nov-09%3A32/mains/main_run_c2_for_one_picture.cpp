#include "headers.h"
#include "C2.h"
#include "C1.h"
#include "image_processing.h"
#include "picture_handler.h"


#define ARGS 2
#define NumberOfPatch 279

int main( int argn , char *argv[]){


	if ( argn < ARGS ){
		cerr<<"number of arguments should be "<<ARGS<<endl;
		return -1;
	}

	int arg_index = 1;

	int base = -45;
	string file_name_picture = argv[arg_index++];
	string file_name_patch = argv[arg_index++];

	Picture test_picture;

	list<list<Picture> > S1_pictures;
	list<list<Picture> > C1_result;
	list<list<Picture> > gabor_filters;
	list<Picture>   patches;
	vector<int> c1SpaceSS,c1ScaleSS;
	int c10L;


	init_parameters_for_C1 (gabor_filters , c1SpaceSS , c1ScaleSS , c10L );

	cout<<"gabor filters\n";
	print_list_list_picture ( gabor_filters );
	cout << "done printing gabor filters" << endl;

	//stringstream picture_address;
	//picture_address << file_name_picture << "Image_back100.txt";
	cout << "before reading picture" << endl;

	//cout << picture_address.str() << endl;
	cout << "file_name_picture" << file_name_picture << endl;
	read_picture_from_file ( test_picture , file_name_picture );

	print_picture(test_picture );	

	cout << "In the End" << endl;
	int TempFinalResult;
	if(C1_Function( test_picture, S1_pictures, C1_result, gabor_filters , c1SpaceSS, c1ScaleSS, c10L) == -1 ){
		cout << "C1_result error  " << endl;
		return -1;
	}

	list<Picture>  ::iterator iter = patches.begin();
	for(int count=0; count < 4; count++){

		stringstream NameOfpatches;
		NameOfpatches << file_name_patch << "patch_" << NumberOfPatch << "(" << base << ").txt";
		read_patch_from_file(*iter , NameOfpatches.str() );
		base += 45;

		cout << "patch #" << count << endl;
		print_picture(*iter);
		iter++ ;
	}
	



	cout << "completed C1 with success" << endl;

	cout<<"S1_pictures\n";
	print_list_list_picture ( S1_pictures );

	cout<<"C1_pictures\n";
	print_list_list_picture ( C1_result );
	cout << "end of C1 pictures" << endl;
	float min;
	//for(list<list<Picture> > ::iterator it1 = patches.begin();it1 != patches.end(); it1++){
	//for(list<Picture> ::iterator it2 = (*it1).begin(); it2 != (*it1).end(); it2++){
	min = 1000000000;
	for(list<list<Picture> >::iterator PicIter = C1_result.begin(); PicIter != C1_result.end(); PicIter++ ){
		Picture distance;
		//cout << "size ::" << PicIter->size() << endl;
		window_patch_distance(*PicIter, patches, distance);
		cout << "after window" << endl;
		float tmp = Min(distance);
		free_picture ( distance );
		min = tmp < min ? tmp : min;

	}

	TempFinalResult = min;
	//	}
	//}


	cout << "here is the result----->" << TempFinalResult << endl; 
	//cout << "printing patches" << endl;



	return 0;

}
