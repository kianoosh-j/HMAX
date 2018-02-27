#ifndef __PICTURE__
#define __PICTURE__

#include "headers.h"
#include "cv.h"
#include "highgui.h"

using namespace cv;

class Picture {

	public:
		Picture(picture_cell_type *picture_matrix , int width , int height , string name);
		Picture(const Picture &picture);
		Picture();
		~Picture();
		int width;
		int height;
		string picture_name;
		picture_cell_type *matrix;
		//newly added for statistics
		//int X;
		//int Y;
		int NumberInDirectory;
		
		//int extract_random_patch(int patch_height, int patch_width, Patch& patch);
		void set_number_in_directory(int number_in_directory);
		int get_number_in_directory();
		void set_matrix ( picture_cell_type *matrix );
		void set_width(int width );
		void set_height(int height );
		picture_cell_type * get_matrix();
		int get_width();
		int get_height();
		// NEWM
		cv::Mat convert_to_opencvMat ();
		// NEWM
		void set_from_opencvMat ( Mat &m );


};

#endif
