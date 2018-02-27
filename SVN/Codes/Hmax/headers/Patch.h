#ifndef __PATCH__
#define __PATCH__

#include "headers.h"
#include "Picture.h"

using namespace std;

class Patch{


	public:
		vector<Picture> patches_in_different_orientations;
		bool extracted_from_train_pictures;
		
		string category_name;
		string picture_name;
		void clear_patch_vector(){
			patches_in_different_orientations.clear();
		}
	//pair< int, int > position_of_extraction;
};
		
#endif
