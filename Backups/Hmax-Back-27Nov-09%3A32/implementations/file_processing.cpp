#include "../headers/headers.h"
#include "../headers/picture_handler.h"
#include "../headers/file_processing.h"

#define HEIGHT_SIZE 140
#define MAX_PIC_MATRIX_SIZE 140*500


int getdir (string base_dir, string name, list<TempType> &files , string basefolder){
	DIR *dp;
	struct dirent *dirp;
	cerr <<"base_dir  " <<  base_dir.c_str() << endl;
	cerr << "base_folder " << basefolder << endl;
	if((dp  = opendir((basefolder+name).c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << basefolder+name << endl;
		return errno;
	}
	cerr << "location of the pictures" <<  basefolder.c_str() << endl;
	ofstream ofs;
	stringstream input_name;
	string dest = base_dir + name + "_NumPicture.txt";
	cerr << "dest is  " << dest << endl;
	//input_name << dir << basefolder << folder;
	//cerr << "dest is " << dest << endl;
	//input_name << basefolder << dest.substr(1,dest.size() - 1);
	input_name << dest; 
	//cout << "base dir  " << base_dir << endl;
	ofs.open(input_name.str().c_str(),ios_base:: out);
	cerr << "ADRESS" << input_name.str().c_str() << endl;
	//return 0;
	string str_temp;
	files.clear();
	ofs << "Name and number of the pictures in the train_pos directory" << endl;
	int count = 0;
	//cerr << "files consists of" << endl;
	while ((dirp = readdir(dp)) != NULL) {
		if ( (str_temp=string(dirp->d_name)) != "." && str_temp != ".." ){
			files.push_back(TempType(string(dirp->d_name),count));				

			ofs << string(dirp->d_name)  << "---------->" << count << endl;
			count++;
	//		cerr << dirp->d_name << endl;
		}


	}
	
	ofs.close();

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
	pic_temp.matrix = &temp_matrix[0];

	init_copy_picture ( *p , pic_temp );

	return 0;

}

int Pixels(string add,Picture *&p,IplImage *&Image1){

	//  cout<<" "<<add<<endl;

	int i,j;
	uchar *ptr;
	IplImage *img;
	IplImage *img1;
	const char *c=add.c_str();

	//cout<<"start loading image\n";
	img1 = cvLoadImage(c,CV_LOAD_IMAGE_GRAYSCALE);
	float factor = img1->height / (float)(HEIGHT_SIZE);
	img = cvCreateImage(cvSize( (int)(img1->width / factor ) , HEIGHT_SIZE ) , img1->depth , 1 );
	cvResize ( img1 , img );

	//cout<<"end of loading image\n";
	if ( !img ){
		cerr<<"error while reading image "<<add<<"\n";
		return -1;
	}
	//cout<<"h = "<<img->height<<" & w = "<<img->width<<endl;
	p->matrix=new float[img->height*img->width]; 

	float *pixel=p->matrix;

	//for(i=0;i<img->height;i++)
	// pixel[i]=new float[img->width];
	//uchar pixel[img->height][img->width];

	for(i=0;i<img->height;i++)
		for(j=0;j<img->width;j++)
		{
			ptr=((uchar *)img->imageData+img->widthStep*i+j);
			pixel[i*img->width+j]=(float)*ptr/255;
		}


	p->height=img->height;
	p->width=img->width;

	cvReleaseImage ( &img1 );

	//cout<<endl<<endl;
	//cout<<p->matrix[0]<<endl;


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
			if ( read_mat((*iter1)+"/"+(*iter21 ).Name,p) != 0 ){
				cerr<<"error : read_images : while calling Pixels\n";
			}
			else{
				p->NumberInDirectory = iter21->Number; 	
				temp.push_back(*p);	
			}
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


