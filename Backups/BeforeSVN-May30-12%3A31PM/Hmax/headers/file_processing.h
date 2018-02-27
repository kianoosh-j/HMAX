#ifndef __FILE_PROCESSING__
#define __FILE_PROCESSING__

#include "headers.h"
#include "cv.h"
#include "highgui.h"

int 	getdir (string base_dir, string name, list<TempType> &files , string basefolder);
int 	read_images ( const list< list<TempType> > &files_name , const list<string> &dirs , list< list<Picture> > &pictures );
int 	write_c2_results ( string base_dir , int sizes[] , int num_of_patches , float **c2_results );
int 	write_c2_results_for_libsvm ( string base_dir , int sizes[] , int num_of_patches , float **c2_results );
int 	read_mat(string add,Picture *&p);
int 	Pixels(string add,Picture *&p,IplImage *&Image1);


#endif
