#include "../headers/headers.h"
#include "../headers/picture_handler.h"


#define MAX_MATRIX_SIZE 1000 * 1000
#define CURRENT_HEIGHT 140

void rand_picture(list<Picture>&pictures,Picture &pic){
	list<Picture>::iterator iter_pictures;
	int reng,random;
	reng = pictures.size();
	random = rand()%reng;
	int count = 0;
	for(iter_pictures=pictures.begin();iter_pictures!=pictures.end();iter_pictures++)
		{
			if(count==random)
			{
				pic=(*iter_pictures);
				count=0;
				break;
			}  
			count++;
		}

}

int max_pictures ( Picture result , Picture first , Picture second ){

	if ( result.width != first.width || result.width != second.width  && result.width < 0){
		cerr<<"not equal width for max pictures\n";
		return -1;
	}

	if ( result.height != first.height || result.height != second.height  && result.height < 0){
		cerr<<"not equal height for max pictures\n";
		return -1;
	}

	for ( int i=0 ; i<result.height ; i++){

		for ( int j=0 ; j<result.width ; j++){

			result.matrix[i*result.width + j] = max ( first.matrix[i*result.width + j] , second.matrix[i*result.width + j] );
		}
	}

	return 0;

}

void free_picture ( Picture &p){

	free(p.matrix);
}

int divide_pictures ( Picture &dest , const Picture &division, const Picture &divisor){

	if ( equal_size_pictures ( dest , division ) && equal_size_pictures ( dest , divisor ) ){
		
		for ( int i=0 ; i<dest.height ; i++ ){
			for ( int j=0 ; j<dest.width ; j++ ){
				int index = i*dest.width + j;
				if ( divisor.matrix[index] == 0 ){
					cerr<<"error : divide_pictures : divide by zero at matrix["<<i<<"]["<<j<<"]\n";
					return -1;
				}
				dest.matrix[index] = division.matrix[index] / divisor.matrix[index];
			}
		}
		return 0;
	}
	else{
		cerr<<"error : divide_pictures : size of pictures are not OK\n";
		return -1;
	}

}

int init_copy_picture ( Picture &dest , Picture &src ){

	if ( src.width < 0 || src.height < 0 ){
		cerr<<"error : init_copy_picture : width and height should be more than zero\n";
		return -1;
	}

	dest.width = src.width;
	dest.height = src.height;
	dest.matrix = new float [dest.width * dest.height];

	for ( int i=0 ; i<src.height ; i++ ){

		for ( int j=0 ; j<src.width ; j++ ){

			dest.matrix[i*src.width+j] = src.matrix[i*src.width+j];
		}
	}
	return 0;

}

bool equal_size_pictures ( const Picture &p1 , const Picture &p2 ){

	if ( !valid_picture ( p1 ) || !valid_picture ( p2 ) ){
		return false;
	}

	if ( p1.width != p2.width || p1.height != p2.height ){
		return false;
	}

	return true;

}

bool valid_picture ( const Picture & p ){


	if ( p.width < 0 || p.height < 0 ){
		cerr<<"error valid picture : width or height is less than zero\n";
		return -1;
	}

	return true;

}

int init_picture ( Picture &p , int w , int h ){

	if ( w < 0 || h < 0 ){
		cerr<<"error : init_picture : w or h is not valid\n";
		return -1;
	}
	p.width = w;
	p.height = h;
	if ( init_picture_matrix (p) ){
		return 0;
	}
	return -1;

}

int init_picture ( Picture &p , int w , int h , int value){

	if ( w < 0 || h < 0 ){
		cerr<<"error : init_picture : w or h is not valid\n";
		return -1;
	}
	p.width = w;
	p.height = h;
	if ( init_picture_matrix (p) ) {
		for ( int i=0 ; i<h ; i++ ){
			for ( int j=0 ; j<w ; j++ ){
				p.matrix[i*w + j] = value;
			}
		}
		return 0;
	}
	return -1;
}

void print_picture (Picture p){

    cout<<"\n\n****************** printing picture \n\n";
    cout<<"\t\tPicture size is : h = "<<p.height<<"  and  w = "<<p.width<<endl<<endl;
    cout<<setprecision(4);
    for(int i=0;i<p.height;i++){
      for(int j=0;j<p.width;j++)
	      printf ( " %20.15lf" , p.matrix[i*p.width+j] );
      cout<<endl;
    }
 

}

bool init_picture_matrix ( Picture &p){

	if ( p.width < 0 || p.height < 0 )
		return false;
	p.matrix =  new float [ p.width * p.height ];
	return true;
}

int abs_matrix ( float *dest , float *src , int size ){

	if ( size < 0 )
		return -1;

	for ( int i=0 ; i<size ; i++ ){

		dest[i] = src[i] > 0 ? src[i] : -src[i];
	}
}

int inverse_picture ( Picture &pic ){

	if ( !valid_picture ( pic ) ){
		cerr<<"error : inverse_picture : picture is not valid\n";
		return -1;
	}

	//cout<<"in inverse : before anythig\n\n";
	//print_matrix ( pic.matrix , pic.width , pic.height );
	for ( int i=0 ; i<(pic.height/2) ; i++ ){
		for ( int j=0 ; j<pic.width ; j++ ){

			int index1 = i*pic.width + j;
			int index2 = pic.width * pic.height - index1 -1;

			//cout<<"index1 = "<<index1<<" , index2 = "<<index2<<endl;
			//cout<<"pic["<<index1<<"] = "<<pic.matrix[index1]<<"  ,  pic["<<index2<<"] = "<<pic.matrix[index2]<<endl;

			float temp = pic.matrix[index1];
			pic.matrix[index1] = pic.matrix[index2];
			pic.matrix[index2] = temp;
			//cout<<"pic["<<index1<<"] = "<<pic.matrix[index1]<<"  ,  pic["<<index2<<"] = "<<pic.matrix[index2]<<endl;
			//cout<<"pic[1] = "<<pic.matrix[index1]<<"  ,  pic[2] = "<<pic.matrix[index2]<<endl;
		}
	}
	//cout<<"in inverse : middle of doing inverse\n\n";
	//print_matrix ( pic.matrix , pic.width , pic.height );
	if ( pic.height %2 != 0 ){
		int i=pic.height/2;
		for ( int j=0 ; j<pic.width/2 ; j++ ){

			int index1 = i*pic.width + j;
			int index2 = pic.width * pic.height - index1 -1;

//			cout<<"index1 = "<<index1<<" , index2 = "<<index2<<endl;
//			cout<<"pic["<<index1<<"] = "<<pic.matrix[index1]<<"  ,  pic["<<index2<<"] = "<<pic.matrix[index2]<<endl;

			float temp = pic.matrix[index1];
			pic.matrix[index1] = pic.matrix[index2];
			pic.matrix[index2] = temp;
//			cout<<"pic["<<index1<<"] = "<<pic.matrix[index1]<<"  ,  pic["<<index2<<"] = "<<pic.matrix[index2]<<endl;
			//cout<<"pic[1] = "<<pic.matrix[index1]<<"  ,  pic[2] = "<<pic.matrix[index2]<<endl;
		}
	}
	//cout<<"in inverse : after doing inverse\n\n";
	//print_matrix ( pic.matrix , pic.width , pic.height );

	return 0;

}

void print_matrix ( float *a , int w , int h ){
	for ( int i=0 ; i<h ; i++ ){
		for ( int j=0 ; j<w ; j++ ){

			printf ( " %8.4f" , a[i*w + j] );
		}
		printf ( "\n" );
	}
}

void read_matrix ( float *a , int w , int h ){

	for ( int i=0 ; i<h ; i++ ){
		for ( int j=0 ; j<w ; j++ ){
			cin>>a[i*w+j];
			//cout<<a[i*w+j];
		}
	}
}

void free_list_list_pictures ( list<list<Picture> > &pictures ){

	for ( list<list<Picture> >::iterator iter_lpicture=pictures.begin() ; iter_lpicture != pictures.end() ; iter_lpicture++ ){
		free_list_pictures ( *iter_lpicture );
		iter_lpicture->clear();
	}
	pictures.clear();

}

void free_list_pictures ( list<Picture> pictures ){

	for ( list<Picture>::iterator iter_picture=pictures.begin() ; iter_picture != pictures.end() ; iter_picture++ ){
		//cerr<<"before freeing picture\n";
		free_picture ( *iter_picture );
		//cerr<<"after freeing picture\n";
	}
	pictures.clear();
}

bool equal_list_pictures ( list<Picture> &pictures ){

	if ( pictures.size() <= 0 ){
		cerr<<"error : equal_list_pictures : list is empty\n";
		return -1;
	}
	Picture pic_temp = *(pictures.begin());


	for ( list<Picture>::iterator iter=pictures.begin() ; iter!=pictures.end() ; iter++){

		if ( !equal_size_pictures ( *iter , pic_temp ) )
			return false;
		pic_temp = *iter;
	}
	return true;
}

int create_random_picture ( Picture &pic , int width , int height ){

	if ( init_picture ( pic , width , height ) != 0 )
		return -1;

	for ( int i=0 ; i<height ;i++){
		for ( int j=0 ; j<width ; j++ ){

			int index = i*width + j;
			pic.matrix[index] = (float)((rand()%999999)+1)/(float)(1000001);
			if ( (rand()%10000) < 369 )
				pic.matrix[index] = -pic.matrix[index];

		}
	}

}

int	read_patch_from_file ( Picture &pic , string file_name ){

	cout << "in read_patch_from_file " << endl;
	fstream fin;

	fin.open ( file_name.c_str()  , ios::in );

	if ( fin.fail() ){

		cerr<<"read_patch_from_file : error while reading file "<<file_name<<endl;
		return -1;
	}


	cout << "opened successfully" << endl;
	float temp_fl;
	char temp_ch;

	int height=0 , width=0;
	int columns_counter, rows_counter;
	rows_counter = CURRENT_HEIGHT;

	float matrix_temp [ MAX_MATRIX_SIZE ];

	int matrix_index = 0;
	while ( fin >> matrix_temp[matrix_index++] ){

	}
	height = rows_counter;

	fin.close();

	Picture pic_temp;
	pic_temp.width = sqrt(matrix_index);
	pic_temp.height = sqrt(matrix_index);
	pic_temp.matrix = &matrix_temp[0];

	init_copy_picture ( pic , pic_temp );

	return 0;

}

int	read_picture_from_file ( Picture &pic , string file_name ){

	cout << "in read_picture_from_file " << endl;
	fstream fin;

	fin.open ( file_name.c_str()  , ios::in );

	if ( fin.fail() ){

		cerr<<"read_picture_from_file : error while reading file "<<file_name<<endl;
		return -1;
	}

	float temp_fl;
	char temp_ch;

	int height=0 , width=0;
	int columns_counter, rows_counter;
	rows_counter = CURRENT_HEIGHT;

	float matrix_temp [ MAX_MATRIX_SIZE ];

	int matrix_index = 0;
	while ( fin >> matrix_temp[matrix_index++] ){

	}
	height = rows_counter;

	fin.close();

	Picture pic_temp;
	pic_temp.width = matrix_index / rows_counter;
	pic_temp.height = height;
	pic_temp.matrix = &matrix_temp[0];

	init_copy_picture ( pic , pic_temp );

	return 0;

}

void print_list_list_picture ( const list<list<Picture> > &llpictures ){


	for ( list<list<Picture> >::const_iterator iter = llpictures.begin() ; iter != llpictures.end() ; iter++ ){

		print_list_picture ( *iter );
	}
}

void print_list_picture ( const list<Picture> &lpictures ){


	for ( list<Picture>::const_iterator iter = lpictures.begin() ; iter != lpictures.end() ; iter++ ){

		print_picture ( *iter );
	}
}
