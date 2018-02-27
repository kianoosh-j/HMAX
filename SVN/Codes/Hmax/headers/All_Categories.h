#ifndef  __ALL_CATEGORIES__
#define  __ALL_CATEGORIES__

#include "Picture_Category.h"

class All_Picture_Categories {

	private:
		vector< vector<Picture_Category> > picture_categories; // first vector refers to each train , test , or test2 and internal vector refers to pictures of all categories for an specific type , for example train pictures 
		int num_of_each_categori_categories; // it is 2 or 3 ( if we have train , test it will be 2 , otherwise for "trian, , test , test2" it will be 3 )
		int num_of_categories ; // for 101 objects it is 101

	public:
		All_Picture_Categories( int num_of_categories , int num_of_each_categori_categories){
			this->num_of_each_categori_categories = num_of_each_categori_categories;
			this->num_of_categories = num_of_categories;
			set_num_of_categories ( num_of_categories );
		}

		All_Picture_Categories(){
		}
		~All_Picture_Categories(){}
		void set_picture_categori ( int index , vector<Picture_Category> &picture_categori ){

			if ( index < picture_categories.size()  ){

				picture_categories [ index ] = picture_categori ;

			}
			else{
				// error TODO
			}
		}

		void add_categori ( int index , Picture_Category &picture_category ){

			if ( index < picture_categories.size() ){

				picture_categories[index].push_back ( picture_category );
			}
			else{

				// error TODO
			}
		}

		void set_num_of_categories ( int num_of_categories ){

			this->num_of_categories = num_of_categories;
		}

		void set_num_of_each_categori_categories ( int num_of_each_categori_categories ){

			if ( num_of_each_categori_categories <= 0 )
				return;
			this->num_of_each_categori_categories = num_of_each_categori_categories;
			picture_categories.resize ( this->num_of_each_categori_categories );
		}

		int get_num_of_each_categori_categories(){
			return this->num_of_each_categori_categories;
		}

};


#endif
