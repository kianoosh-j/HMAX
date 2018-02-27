#include "headers/headers.h"
#include "headers/C1.h"
#include "headers/C2.h"
#include "headers/file_processing.h"
#include "headers/svm.h"
#include "headers/pthread_headers.h"
#include "pthread.h"
#include "../SVM/headers/libsvm.h"

#define ARGN 4
#define DIR_NUMS 4


int main( int argn , char *argv[]){
	


	int debug_mode;
	if ( argn < ARGN ){
		cerr<<" Arguments is not correct, please run the program as below:\n\n\t./hmax    picture_dirs\n\n";
		return 0;
	}
	int base_index=1;
	debug_mode = atoi(argv[base_index++] );


	//string number = argv[base_index++];
	string base_folder = argv[base_index++];

	list< list<TempType> > files_name;
	list<TempType> files_name_temp;
	list< list<Picture> > pictures;
	list< list< list<Picture> > > patches;
	list<string> dirs;
	list<int> patch_sizes;
	int num_of_patches_for_each_image = 250;
	int num_of_patches = 250 * 4; //this should be calculated from a formula
	patch_sizes.push_back(4), patch_sizes.push_back(8), patch_sizes.push_back(12), patch_sizes.push_back(16); 
	string destination_dir;


	vector<string>input_name ;
	input_name.push_back("train_pos");
	input_name.push_back("train_neg");
	input_name.push_back("test_pos");
	input_name.push_back("test_neg");

	

	cout<<" before reading directories in make_face :  "<<endl;
	for ( int i=0 ; i<input_name.size() ; i++ ){
	
	destination_dir = argv[base_index];
	cout << "argv[base_index]+input_name[i] " << base_folder+input_name[i] << endl;	
		dirs.push_back ( base_folder + input_name[i] );
		if ( getdir ( destination_dir.c_str() , input_name[i] , files_name_temp , base_folder ) != 0 ){
			cerr<<"error in reading files of dir "<<base_folder+input_name[i]<<endl;
			return -1;
		}
		cout << "out of getdir()" << endl;
		files_name.push_back (files_name_temp) ;
	}
	
		base_index++;
	cerr<<"successfully reading dirs\n";

	if ( read_images ( files_name , dirs , pictures ) != 0 ){
	       	cerr<<"error in reading images\n\n";
		return -1;
	}

	cerr<< " after read images\n";
	srand(time(0));
	extract_patches ( *(pictures.begin()) , patches , (pictures.begin()->size() ) , patch_sizes , num_of_patches_for_each_image );
	return 0;
	cerr<< " after extract patches\n";
	string degree[4] = {"90","-45","0","45"};
	int counter = 0;

	for(list<list<list<Picture> > >::iterator iter = patches.begin();iter != patches.end();iter++){

		for(list<list<Picture> >::iterator iter2 = iter->begin();iter2 != iter->end();iter2 ++){
			int deg = 0;
			counter++;
			for(list<Picture>::iterator iter3 = iter2->begin();iter3 != iter2->end();iter3++){
				ofstream ofs;
				stringstream input_name;
				input_name<< destination_dir << "patches/patch_"<<counter<<"("<<degree[deg]<<").txt";
				ofs.open(input_name.str().c_str(),ios_base:: out);
				for(int i=0;i<iter3->height;i++){
			
					for(int j=0;j<iter3->width;j++)
					{
						ofs<<setprecision(6)<<iter3->matrix[i*iter3->width+j]<<"\t";
					}


				cout<<endl;
				}
				
				ofs.close();
				ofstream ofs2;
				stringstream input_name2;
				input_name2<< destination_dir << "patches/patch_additional_info_"<<counter<<"("<<degree[deg]<<").txt";
				ofs2.open(input_name2.str().c_str(),ios_base:: out);
				
				ofs2 << "\nNumber of parent picture in directory:\t" << iter3->NumberInDirectory << endl << "Index in parent picture\t(" << iter3->X << "," << iter3->Y << ")\n" ;
				deg++;
				ofs2.close();
			}

		}
	}
	
	vector<vector<vector<float> > > C2_results;

	allocate_c2_result (  pictures , C2_results	,  num_of_patches );
	cerr<<"start calling C2 for all pictures\n";
	run_c2_for_all_pictures ( pictures , patches , C2_results );
	cerr<<"end of calling C2 for all pictures\n";
	
	float **C2_results_fl;
	int *sizes = new int [ pictures.size() ];

	list<list<Picture> >::iterator iter_lpicture = pictures.begin();
	int total_pictures=0;
	
	for ( int i=0 ; i<pictures.size() ; i++){
		sizes[i] = iter_lpicture->size();
		iter_lpicture++;
		total_pictures += sizes[i];
	}
	C2_results_fl = new float * [ total_pictures ];

	vector<vector<vector<float> > >::iterator iter_vvfloat;
	vector<vector<float> >::iterator iter_vfloat;
	vector<float>::iterator iter_float;
	int temp_index=0;
	for ( iter_vvfloat=C2_results.begin() ; iter_vvfloat != C2_results.end() ; iter_vvfloat++){
		for ( iter_vfloat=iter_vvfloat->begin() ; iter_vfloat!=iter_vvfloat->end() ; iter_vfloat++){
			C2_results_fl[temp_index] = new float [num_of_patches_for_each_image*patch_sizes.size() ];
			int temp_index2=0;
			
			cout << "$$$$$$$$$$$$$$$$$$" << endl ;
			for ( iter_float = iter_vfloat->begin() ; iter_float != iter_vfloat->end() ; iter_float++){
				C2_results_fl[temp_index][temp_index2] = *iter_float;
				temp_index2++;
			}
			cout << "$$$$$$$$$$$$$$$$$$$$" << endl;
			cerr<<endl;
			temp_index++;
		}
	
	}
	cout << "final temp index:" << temp_index << endl;
	
	cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ C2 Results\n\n";
	for(int i=0; i<total_pictures; i++)
		for(int j=0; j< num_of_patches; j++) 
			cout<<setprecision(12)<< C2_results_fl[i][j]<<endl;
	cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ C2 Results\n\n";

	int test_pos_size = sizes[2];
	int test_neg_size = sizes[3];
	float *final_results = new float [ test_pos_size  + test_neg_size ];

			
	cerr << "before SVM" << endl;
	//svm ( C2_results_fl , sizes[0],sizes[1],sizes[2],sizes[3], patch_sizes.size() * num_of_patches_for_each_image, final_results );
	vector<int> vsizes(4);
	for ( int i=0 ; i<4 ; i++ ){
		vsizes[i] = sizes[i];
	}
	svm_libsvm ( C2_results_fl , vsizes , patch_sizes.size() * num_of_patches_for_each_image, final_results );
	cerr << "after SVM" << endl;

	int success_rate=0;

	cerr<<"\n\n**********************************************************\n\n                     Pos results\n\n**********************************************************\n\n"; 
	cerr<<"kianoosh main test test_pos_size "<<test_pos_size <<endl;
	for ( int i=0 ; i<test_pos_size ; i++){
		cerr<<final_results[i]<<endl;
		if ( final_results[i]==1 )
			success_rate++;
	}

	cerr<<"\n\n**********************************************************\n\n                     Neg results\n\n**********************************************************\n\n"; 
	for ( int i=0 ; i<test_neg_size ; i++){
		cerr<<final_results[i+test_pos_size]<<endl;
		if ( final_results[i+test_pos_size]==-1 )
			success_rate++;
	}
	cerr<<"\n\n***********************************************************\n\n\t\tSuccess rate is :\t"<<float(success_rate)*100/(test_pos_size+test_neg_size)<<endl<<endl;


	write_c2_results_for_libsvm ( destination_dir , sizes , num_of_patches_for_each_image*patch_sizes.size() , C2_results_fl );
	write_c2_results ( destination_dir , sizes , num_of_patches_for_each_image*patch_sizes.size() , C2_results_fl );
	cerr << "after write_c2_results" << endl;
	ofstream ofs1;
	stringstream input_name1;
	input_name1<< destination_dir <<"/success_rate.txt";
	ofs1.open(input_name1.str().c_str(),ios_base:: out);
	ofs1<<float(success_rate)*100/(test_pos_size+test_neg_size)<<endl;
	return 0;
}

