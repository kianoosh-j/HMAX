#include "headers.h"
#include "picture_handler.h"
#include "file_processing.h"
#include "errors.h"
#include "Patch.h"
#include <set>

#define HEIGHT_SIZE 140
#define MAX_PIC_MATRIX_SIZE 140*500

const string slash_str = "/";
const string file_name = "file_processing.cpp";

int getdir (string base_dir ,list<TempType> &files ){


	// necessary temporal variables
	DIR *dp;			// directory pointer
	struct dirent *dirp;		// for files in the directory
	string file_name_str_temp;	// for getting file name string


	// open directory to read all files and directories
	if( (dp= opendir((base_dir.c_str()))) == NULL) {
		cout << "Error: getdir : opening " << base_dir<< endl;
		return -1;
	}


	// give a file number starting from 0 to every file or directory we read
	int file_number = 0;

	// read all files and directories using directory pointer (dp) except . and ..
	while ((dirp = readdir(dp)) != NULL) {
		
		// check file name was not . and ..
		if ( (file_name_str_temp=string(dirp->d_name)) != "." && file_name_str_temp != ".." ){

			files.push_back(TempType(string(dirp->d_name),file_number));				
			file_number++;

		}
	}

	closedir(dp);
	return 0;
}


int read_mat(string add,Picture *&p){

	float temp_matrix [MAX_PIC_MATRIX_SIZE];

	int matrix_size;

	fstream fin;
	fin.open ( add.c_str() , ios::in );

	float pixel_temp;

	int matrix_index=0;
	while ( fin>>temp_matrix[matrix_index++]){
	}

	fin.close();

	int base_height = 140;
	Picture pic_temp;
	pic_temp.height = base_height;
	pic_temp.width = matrix_index/base_height;
	pic_temp.matrix = temp_matrix;

	init_copy_picture ( *p , pic_temp );

	return 0;

}


int picture_pixels_normalization( IplImage* img, Picture& picture ){

	
	uchar *tmp;

	//initializing the picture parameters	
	picture.matrix=new float[img->height * img->width]; 
	picture.height=img->height;
	picture.width=img->width;

	//finalizing the picture's pixels 
	for(int i=0; i < img->height; i++)
		for(int j=0; j < img->width; j++)
		{
			tmp=((uchar *) img->imageData + img->widthStep * i + j);
			picture.matrix[ i * img->width + j]=(float) *tmp / 255;
		}

	return 0;

}


// this function reads all images using dirs/files_name and store it in pictures
int read_images ( const list< list<TempType> > &files_name , const list<string> &dirs , list< list<Picture> > &pictures ){


	int count2=0,count1=0;
	Picture *p=new Picture;


	list<Picture> temp;
	list<list<TempType> >::const_iterator iter2;
	list<TempType>::const_iterator iter21;
	list<string>::const_iterator iter1;
	iter1=dirs.begin();
	iter2=files_name.begin();
	
		IplImage *img;

	while(iter1!=dirs.end()&&iter2!=files_name.end())
	{
		iter21=(*iter2).begin();


		while(iter21!=(*iter2).end())
		{
			//TODO Pixels should be implemented
			/*if ( Pixels ( (*iter1)+"/"+(*iter21 ).Name , p , img ) != 0 ){
				cerr<<"error : read_images : while calling Pixels\n";
			}
			else{
				p->NumberInDirectory = iter21->Number; 	
				temp.push_back(*p);	
			}
			*/
			iter21++;
		}
		pictures.push_back(temp);




		temp.clear();
		iter1++;
		iter2++;
	}

	return 0;
}


int write_c2_results ( string base_dir , int sizes[] , int num_of_patches , float **c2_results ){

	vector<string>posts(0);
	posts.push_back ("C2Results_train_pos.txt" );
	posts.push_back ("C2Results_train_neg.txt" );
	posts.push_back ("C2Results_test_pos.txt" );
	posts.push_back ("C2Results_test_neg.txt" );

	fstream ofs;

	int index = 0;
	for ( int i=0 ; i<4 ; i++ ){

		ofs.open ( (base_dir+posts[i]).c_str() , ios::out );

		for ( int j=0 ; j<sizes[i] ; j++){

			for ( int k=0 ; k<num_of_patches  ; k++){
				ofs<<c2_results[j+index][k]<<" ";
			}
			ofs<<"\n";
		}
		index += sizes[i];
		ofs.close();
	}
}


int write_c2_results_for_libsvm ( string base_dir , int sizes[] , int num_of_patches , float **c2_results ){

	vector<string>posts(0);
	posts.push_back ("C2Results_train_pos_libsvm.txt" );
	posts.push_back ("C2Results_train_neg_libsvm.txt" );
	posts.push_back ("C2Results_test_pos_libsvm.txt" );
	posts.push_back ("C2Results_test_neg_libsvm.txt" );

	fstream ofs;

	int index = 0;
	for ( int i=0 ; i<4 ; i++ ){
		stringstream name;
		name<<base_dir+posts[i];

		ofs.open (name.str().c_str(), ios::out );

		for ( int j=0 ; j<sizes[i] ; j++){
			if(i==0 || i==2)
				ofs<<"1 ";
			else 
				ofs<<"-1 ";

			for ( int k=0 ; k<num_of_patches  ; k++){
				ofs<<k<<":"<<c2_results[j+index][k]<<" ";
			}
			ofs<<"\n";
		}
		index += sizes[i];
		ofs.close();
	}
}


int read_image ( string image_file_name , Picture &p ){

	// TODO you should check if image_file_name does not have supporting formats , using get_supporting formats


	// 1- loading image from file , using opencv library and check if it has been loaded with no problem
	IplImage *img = cvLoadImage(image_file_name.c_str() , CV_LOAD_IMAGE_GRAYSCALE);
	if ( img == NULL ){
		cerr << " file_processing.cpp : read_image : can not load image \" "<<image_file_name<<"\n";
		return -1;
	}

	// allocate memory for Picture and move data from Mat Class to our Matrix Defined
	picture_cell_type *matrix=new picture_cell_type [img->height*img->width];
	for(int i=0;i<img->height;i++)
		for(int j=0;j<img->width;j++)
		{
			uchar *ptr=((uchar *)img->imageData+img->widthStep*i+j);
			matrix[i*img->width+j]=(double)*ptr/255;
		}


	// assign image to Picture p , which is call by reference;
	p.set_matrix ( matrix );
	p.set_height ( img->height );
	p.set_width ( img->width );

	return 0;
}


int read_directory_pictures ( string directory , vector<Picture> &pictures ){

	// get all file names from directory
	list<TempType> picture_file_names;
	if ( getdir ( directory , picture_file_names ) != 0 ){
		show_error ( file_name , "read_directory_pictures" , "error while reading file names form directory " + directory );
		return -1;
	}


	// clear the vector
	pictures.clear();

	// for each picture, test the format (substring after .) and if OK , read it and push it in vector
	for ( list<TempType>::iterator iter = picture_file_names.begin() ; iter != picture_file_names.end() ; iter++ ){

		string picture_file_name = iter->Name;
		string image_full_path = directory + slash_str + picture_file_name ;
		Picture picture;

		// check if format is accepted
		if ( check_image_file_format ( picture_file_name ) ) {

			// read picture and push back it to pictures
			if ( read_image ( image_full_path , picture ) != 0 ){

				show_error ( file_name , "read_directory_pictures" , " error in reading one of pictures with name " + picture_file_name );
				return -1;
			}
			pictures.push_back(picture);
		}
	}
	return 0;//TODO
}


int read_all_101_images(string picture_directory, All_Picture_Categories &all_101_categories  ){


	// get list of directoris in this directory
	list<TempType> directories;
	if(getdir(picture_directory,directories) != 0 ){
		cerr<<"file_proceccing.cpp : read_all_101_images : error in reading directories of dir base_dir : "<<picture_directory;
		return -1;
	}

	// set all_101_categories num of types ( 2 or 3 depend on type )
	int num_of_types = all_101_categories.get_num_of_each_categori_categories();
	if ( num_of_types < 2 ) 
		return -1; // ERROR TODO

	// set vector
	vector<string> types ;
	types.resize(num_of_types);
	types[0] = "train";
	types[1] = "test";
	if ( num_of_types > 2 )
		types[2] = "test2";


	// for each directory read_all pictures and set it to all_101_categories
	int category_label = 0;
	for ( list<TempType>::iterator iter = directories.begin() ; iter != directories.end() ; iter++ , category_label++ ){

		for ( int i=0 ; i<num_of_types ; i++ ){

			Picture_Category picture_category (iter->Name , category_label);

			if ( read_category_pictures ( picture_directory + slash_str + types[i] , picture_category) != 0 ){
				cerr<<"fie_processing.cpp : read_all_101_images : error in reading one of categories";
				return -1;
			}

			all_101_categories.add_categori ( i , picture_category );
		}

	}

	return 0;

}


int  read_category_pictures ( string category_directory , Picture_Category &picture_category){

	if ( read_directory_pictures ( category_directory  , *(picture_category.get_pictures_reference()) ) != 0 ){
		return -1;
	}
	
	return 0;
}


/*int read_all_101_images(string picture_directory, map<int,list<string>> &trains, map<int ,list<string>> &tests ){
	

	// files_name contain file names of all picture files , it is list of list as we have 101 types of pictures,
	list<TempType>files_name;              ////list of 101 object
	list<TempType>train_names;                ////list of all of train pictures in 101 object
	list<TempType>test_names;                ////list of all of test pictures in 101 object
	list<string> dirs;


	vector<string>input_name ;
	input_name.push_back("/train");
	input_name.push_back("/test");

	// if base_dir does not contain   "slash"  put it at the end of it
	// warning : it is for windows and not linux, so it would be better that base_dir contain it
	if ( picture_directory[picture_directory.size()-1] != '/' ){
		picture_directory.push_back('/');
	}


	// for each of 101 folders gets all images 
	

	for(list<TempType>::iterator iter_obj = files_name.begin(); iter_obj != files_name.end().end(); iter_obj++){
		string temp_train =  picture_directory + (*iter_obj).Name + input_name[0];
		getdir(temp_train,train_names);
		for(list<TempType>::iterator iter_train = train_names.begin(); iter_train != train_names.end(); iter_train++){
			trains.insert(pair<int,list<string> >((*iter_obj).Number),(*iter_train).Name);
		}

		string temp_test =  picture_directory + (*iter_obj).Name + input_name[1];
		getdir( temp_test,test_names);
		for(list<TempType>::iterator iter_test = test_names.begin(); iter_test != test_names.end(); iter_test++){
			tests.insert(pair<int,list<string> >((*iter_obj).Number),(*iter_test).Name);
		}


	}
	dirs.push_back(temp_train);
	dirs.push_back(temp_test);

	if ( read_images ( files_name , dirs , pictures ) != 0 ){
	       	cerr<<"error in reading images\n\n";
		return -1;
	}

	return 0;

}*/


// write patches information to files
// it has assumed that degree order is 90, -45 0 45
// patches is list of list of list
// 	1- first list i
int write_patches_information_to_file ( string destination_dir,  list<list< Patch > > &patches , int number_of_patches ){
	
	// it was implemented before , but as we wanted to move it to new class an we had it in svn server , we removed the code , 
	//TODO
	return -1;
}


int ReadPatches(vector<float> &patches){

	int size = patches.size();
	int num_of_patches_persize;
	vector<int > psize(4);
	fstream visual_patch;
	visual_patch.open("visual_dictionary.out");
	return -1;

}
	


// check if format of picture file name is acceptable
bool check_image_file_format ( string image_file_name ){

	static set<string> image_formats;
	image_formats.insert ( "jpg" );
	image_formats.insert ( "JPG" );
	image_formats.insert ( "png" );
	image_formats.insert ( "PNG" );


	if ( image_file_name.size() > 4 
			&& image_file_name.find('.') != -1  // it has . for getting the suffix
			&& image_formats.find(image_file_name.substr(image_file_name.find('.')+1)) != image_formats.end() ) // the format ( substr after "." is in formats set 

		return true;

	else
		return false;
}




















