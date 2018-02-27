#ifndef __PICTURE_CATEGORY__
#define __PICTURE_CATEGORY__

#include <list>
#include <vector>
#include "Picture.h"

using namespace std;

class Picture_Category{

	private:
		vector<Picture> pictures;
		string category_name;
		int category_label;
	public:
		Picture_Category(string category_name , int category_label);
		Picture_Category();
		~Picture_Category();
		void set_Category_Label(int category_label);
		void add_Picture(Picture &picture);
		vector<Picture> *get_pictures_reference();

};


#endif
