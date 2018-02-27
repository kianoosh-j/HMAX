#include "headers.h"
#include "get_files.cpp"
#include "read_images.cpp"
#include "image_processing.h"
#include "C2.h"
#include "svm.h"
#include "myconv.h"
#include "pthread_headers.h"
#include "pthread.h"

#define ARGN 5
#define DIR_NUMS 4


int allocate_c2_result ( list<list<Picture> >&pictures , vector<vector<vector<float> > > &C2_result , int num_of_patches);

int main( int argn , char *argv[]){


	Picture pic;
	list<Picture> pictures;
	list<Picture> patches;
	int width_pic=21,height_pic=30, width_patch=8,height_patch=8;
	for ( int i=0 ; i<4 ; i++ ){
		create_random_picture ( pic , width_pic , height_pic );
		pictures.push_back (pic);
		create_random_picture ( pic , width_patch , height_patch );
		patches.push_back (pic);
	}

	Picture distance;
	window_patch_distance ( pictures , patches , distance );

	int i=1;
	cout<<"Mahmoud "<<pictures.size()<<patches.size()<<"\n\n";
	for ( list<Picture>::iterator iterp=pictures.begin() ; iterp != pictures.end() ; iterp++){
		cout<<"Picture "<<i<<endl;
		print_picture ( *iterp );
		i++;
	}
	i=1;
	for ( list<Picture>::iterator iterp=patches.begin() ; iterp != patches.end() ; iterp++){
		cout<<"Patches "<<i<<endl;
		print_picture ( *iterp );
		i++;
	}

	cout<<"********************	Distance	*****************************\n";
	print_picture ( distance );

	free_list_pictures ( pictures );
	free_list_pictures ( patches );
	free_picture ( distance );

	return 0;

}
