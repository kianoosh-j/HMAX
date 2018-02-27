#ifndef __FILE_PROCESSING__
#define __FILE_PROCESSING__

#include "headers.h"
#include "Picture.h"
#include "Picture_Category.h"
#include "cv.h"
#include "highgui.h"
#include "All_Categories.h"

int 	getdir (string base_dir ,list<TempType> &files );
int 	read_images ( const list< list<TempType> > &files_name , const list<string> &dirs , list< list<Picture> > &pictures );
int 	write_c2_results ( string base_dir , int sizes[] , int num_of_patches , float **c2_results );
int 	write_c2_results_for_libsvm ( string base_dir , int sizes[] , int num_of_patches , float **c2_results );
int 	read_mat(string add,Picture *&p);


// this function read all 101 objects from directory and put it in list of Picture_Category class 
// NEWM
int read_all_101_images(string picture_directory, All_Picture_Categories &all_101_categories );

// this function read pictures of one category from directory containig the 2 sub directories : one for train and one for test
// NEWM
int  read_category_pictures ( string category_directory , Picture_Category &picture_category);

// this function read pictures of one directory and puts it vector<Picture>
// NEWM
int read_directory_pictures ( string directory , vector<Picture> &pictures );

// write patches to files
int write_patches_information_to_file ( string destination_dir,  list<list< Patch > > &patches );

// init picture pixels and normalization
int picture_pixels_normalization( IplImage* img, Picture& picture );

// read and image from file ( with different formats like jpg png ... )
// NEWM
int read_image ( string image_file_name , Picture &p );

// it checks wheter format of an image file ( according to name ) is supported
// NEWM
bool check_image_file_format ( string image_file_name );

#endif
