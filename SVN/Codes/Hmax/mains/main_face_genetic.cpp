#include "headers.h"
#include "get_files.h"
#include "read_images.h"
#include "image_processing.h"
#include "C2.h"
#include "svm.h"
#include "myconv.h"
#include "pthread_headers.h"
#include "pthread.h"
#include <ga/GASimpleGA.h>
#include <ga/GABin2DecGenome.h>

float** C2_results_fl;
int *sizes;
int SIZE;
float* final_results;

#define ARGN 8
#define DIR_NUMS 4


int allocate_c2_result ( list<list<Picture> >&pictures , vector<vector<vector<float> > > &C2_result , int num_of_patches);

int main( int argn , char *argv[]){


    
	int debug_mode;
	if ( argn < ARGN ){
		cerr<<" Arguments is not correct, please run the program as below:\n\n\t./hmax    picture_dirs\n\n";
		return 0;
	}
	debug_mode = atoi(argv[1] );

	list< list<string> > files_name;
	list<string> files_name_temp;
	list< list<Picture> > pictures;
	list< list< list<Picture> > > patches;
	list<string> dirs;
	list<int> patch_sizes;
	int num_of_patches_for_each_image = 250;
	int num_of_patches = 250 * 4; //this should be calculated from a formula
	patch_sizes.push_back(4), patch_sizes.push_back(8), patch_sizes.push_back(12), patch_sizes.push_back(16); 


	vector<string>input_name ;
	input_name.push_back("train_pos");
	input_name.push_back("train_neg");

	input_name.push_back("test_pos");
	input_name.push_back("test_neg");


	cout<<" before reding directories in make_face :  "<<endl;
	for ( int i=0 ; i<DIR_NUMS ; i++ ){

		dirs.push_back ( argv[2]+input_name[i] );
		if ( getdir ( argv[2]+input_name[i] , files_name_temp ) != 0 ){
			cerr<<"error in reading files of dir "<<argv[2]+input_name[i]<<endl;
			return -1;
		}
		files_name.push_back (files_name_temp) ;
	}

	cout<<"successfully reading dirs\n";
	//return 0;

	if ( read_images ( files_name , dirs , pictures ) != 0 ){ cerr<<"error in reading images\n\n";
		return -1;
	}

	cerr<< " after read images\n";
	srand(time(0));
	extract_patches ( *(pictures.begin()) , patches , (pictures.begin()->size() ) , patch_sizes , num_of_patches_for_each_image );
	cerr<< " after extract patches\n";


	vector<vector<vector<float> > > C2_results;

	allocate_c2_result (  pictures , C2_results	,  num_of_patches );
	cerr<<"start calling C2 for all pictures\n";
	run_c2_for_all_pictures ( pictures , patches , C2_results );
	cerr<<"end of calling C2 for all pictures\n";
	string degree[4] = {"0","45","90","135"};
	int counter=0;
	for(list<list<list<Picture> > >::iterator iter = patches.begin();iter != patches.end();iter++){
		for(list<list<Picture> >::iterator iter2 = iter->begin();iter2 != iter->end();iter2 ++){
			int deg=0;
			counter++;
			for(list<Picture>::iterator iter3 = iter2->begin();iter3 != iter2->end();iter3++){
				
			
				ofstream ofs;
				Picture pic=*iter3;
				stringstream input_name;
				input_name<<"./patches/faces/patch"<<counter<<"_"<<degree[deg]<<".txt";
				ofs.open(input_name.str().c_str(),ios_base:: out);
				for(int i=0;i<pic.height;i++){
					for(int j=0;j<pic.width;j++)
					{
						ofs<<setprecision(6)<<pic.matrix[i*pic.width+j]<<"  ";
					}
					ofs<<"\n";
				}

 
				ofs.close();
				deg++;

			}
				
		}
	}
	


	float **C2_results_fl;
	sizes = new int [ pictures.size() ];

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
				cout<<"array elemets:\t" << *iter_float<<"temp_index:\t" << temp_index << "temp_index2:\t" << temp_index2 <<"\n";
				temp_index2++;
			}
			cout << "$$$$$$$$$$$$$$$$$$$$" << endl;
			cerr<<endl;
			temp_index++;
		}

	}
	cout << "final temp index:" << temp_index << endl;
	string output_name = argv[3];
	FILE *C2_res_train_pos = fopen(argv[3],"w");
	FILE *C2_res_train_neg = fopen(argv[4],"w");
	FILE *C2_res_test_pos = fopen(argv[5],"w");
	FILE *C2_res_test_neg = fopen(argv[6],"w");


	for(int i=0; i<4; i++){
		for(int j=0; j<1000; j++){
			for(int k=0; k<sizes[i]; k++){
				if(i==0){
					fprintf(C2_res_train_pos,"\t%10.12f",C2_results_fl[k][j]);	
				}
				if(i==1){
					fprintf(C2_res_train_neg,"\t%10.12f",C2_results_fl[k][j]);	
				}
				if(i==2){
					fprintf(C2_res_test_pos,"\t%10.12f",C2_results_fl[k][j]);	
				}
				if(i==3){
					fprintf(C2_res_test_neg,"\t%10.12f",C2_results_fl[k][j]);	
				}
			}
			if(i==0){
				fprintf(C2_res_train_pos,"\n");	
			}
			if(i==1){
				fprintf(C2_res_train_neg,"\n");	
			}
			if(i==2){
				fprintf(C2_res_test_pos,"\n");	
			}
			if(i==3){
				fprintf(C2_res_test_neg,"\n");	
				//C2_res_test_neg<<setprecision(12)<<C2_results_fl[j][k]<<endl;
			}


		}
	}
	fclose(C2_res_train_pos);	
	fclose(C2_res_train_neg);	
	fclose(C2_res_test_pos);	
	fclose(C2_res_test_neg);	


	int test_pos_size = sizes[2];
	int test_neg_size = sizes[3];
	final_results = new float [ test_pos_size  + test_neg_size ];
    SIZE = patch_sizes.size() * num_of_patches_for_each_image;
	//	cout << "SSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVMMMMMMMMMMMMMMMMMMMMMM" << endl;
	//	for(int i = 0;i <  patch_sizes.size() * num_of_patches_for_each_image;i++ )
	//		for(int j =0; j < sizes[0] + sizes[1] + sizes[2] + sizes[3]; j++)
	//cerr << dec << C2_results_fl << " ";	

    
    
    //******************
     int width = 1000;
     int height = 1;
     int popsize = 10;
     int ngen = 200;
     float pmut = 0.001;
     float pcross = 0.9;
     
     
     GA2DBinaryStringGenome genome(width, height, Objective);
     
     GASimpleGA ga(genome);
     ga.populationSize(popsize);
     ga.nGenerations(ngen);
     ga.pMutation(pmut);
     ga.pCrossover(pcross);
     ga.evolve();
     //*****
    
    cout << ga.statistics().bestIndividual() << endl;
    
    
    
	//	cout <<"AFTER SSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVvMMMMMMMMMMMMMMMMM" << endl;

    
    
	return 0;

}

int allocate_c2_result ( list<list<Picture> >&pictures , vector<vector<vector<float> > > &C2_result , int num_of_patches){

	C2_result.resize ( pictures.size() );
	int index=0;

	for ( list<list<Picture> >::iterator iter_lpicture = pictures.begin() ; iter_lpicture != pictures.end() ; iter_lpicture++){
		C2_result[index].resize ( iter_lpicture->size() );
		for ( int i=0 ; i<C2_result[index].size() ; i++){
			C2_result[index][i].resize ( num_of_patches );
		}
		index++;
	}
	return 0;

}

float Objective( GAGenome& g ){
    
    
	svm ( C2_results_fl , sizes[0],sizes[1],sizes[2],sizes[3], patch_sizes.size() * num_of_patches_for_each_image, final_results );
    
    int success_rate=0;
    
	cout<<"\n\n**********************************************************\n\n                     Pos results\n\n**********************************************************\n\n"; 
	//cout<<"kianoosh main test test_pos_size "<<test_pos_size <<endl;
	for ( int i=0 ; i<test_pos_size ; i++){
		cout<<final_results[i]<<endl;
		if ( final_results[i]==1 )
			success_rate++;
	}
    
	cout<<"\n\n**********************************************************\n\n                     Neg results\n\n**********************************************************\n\n"; 
	for ( int i=0 ; i<test_neg_size ; i++){
		cout<<final_results[i+test_pos_size]<<endl;
		if ( final_results[i+test_pos_size]==-1 )
			success_rate++;
	}
	return(float(success_rate)*100/(test_pos_size+test_neg_size));
	cout <<"****************************" << endl << "Reached the end of one generation " << endl;
    
    
}
