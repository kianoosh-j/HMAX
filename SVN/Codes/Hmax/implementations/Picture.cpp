#include "Picture.h"


Picture::Picture(picture_cell_type *picture_matrix , int width , int height , string name){
	this->width = width;
	this->height = height;
	this->matrix = picture_matrix;
	this->picture_name = name;
}


Picture::Picture(const Picture &picture){
	this->width = picture.width;
	this->height = picture.height;
	this->matrix = picture.matrix;
	this->picture_name = picture.picture_name;
}


Picture::~Picture(){
}


Picture::Picture(){
}

void Picture::set_number_in_directory(int number_in_directory){
	this->NumberInDirectory = number_in_directory;
}
int Picture::get_number_in_directory(){
	return this->NumberInDirectory;
}

//this function extracts a random patch from itself
/*int Picture::extract_random_patch(int patch_height, int patch_width, Patch& patch){

	list<list<Picture> > gabor_filters;
	list<list<Picture> > S1_Pictures;
	list<list<Picture> > C1_Pictures;
	vector<int> c1SpaceSS, c1ScaleSS;
	int c10L;
	int rotations = 4; 
	Picture temp_patch;

	init_parameters_for_C1 ( gabor_filters , c1SpaceSS , c1ScaleSS , c10L );

	C1_Function ( *this, S1_Pictures, C1_Pictures , gabor_filters, c1SpaceSS, c1ScaleSS, c10L );

	//C1_Pictures has only one list of elements here
	list<list<Picture> >::iterator list_of_pictures_in_different_orientations = C1_Pictures.begin();

	for ( int i=0 ; i< rotations ; i++ ){

		init_picture( temp_patch, patch_width, patch_height );
		patch.patches_in_different_orientations.push_back(temp_patch);
	}



	return (extract_arandom_patch( *list_of_pictures_in_different_orientations , patch ));

}*/


void Picture::set_matrix ( picture_cell_type *matrix ){
	this->matrix = matrix;
}
void Picture::set_width(int width ){
	this->width = width;
}
void Picture::set_height(int height ){
	this->height = height;
}
picture_cell_type * Picture::get_matrix(){
	return matrix;
}
int Picture::get_width(){
	return width;
}
int Picture::get_height(){
	return height;
}


// this method convert Picture to opencv Mat
cv::Mat Picture::convert_to_opencvMat (){

	Mat opencv_image;
	opencv_image = Mat ( height , width , DataType<picture_cell_type>::type , matrix );
	return opencv_image;
}

// this method gets opencv Mat and save it to picture
void Picture::set_from_opencvMat ( Mat &m ){

	//  first check if this Mat is OK for converting to Picture Class ( Type should be picture_cell_type and depth should be 1 )
	if ( DataType<picture_cell_type>::type == m.type() // check type
			&& m.channels() == 1 // depth is 1
			){

		this->width = m.size().width;
		this->height = m.size().height;
		this->matrix = new picture_cell_type [ this->width * this->height ];
		memcpy ( this->matrix , m.data , this->width * this->height * sizeof(picture_cell_type) );

	}

}