#include "libsvm.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

#define ARGN 3

using namespace std;


int read_c2_results ( string c2_file , float **c2_results , int start_index , int size , int num_of_patches );
void print_c2_results ( float **c2_results , vector<int> sizes  , int num_of_patches);

int main( int argn , char *argv[] ){

	cout<<"test libsvm hello\n";


	vector<string> c2_files(4);
	vector<int> sizes(4);
	int total_size;
	float **c2_results;
	float *final_results;
	int num_of_patches;
	int total_tests;
	int test_pos , test_neg;
	int success_predicts;

	if ( argn < ARGN ){

		cerr<<"please call the program like this\n";
		cerr<<"\n\t./libsvm-test    base_c2_files   sizes_file   num_of_patches\n\n";
		return -1;
	} 

	c2_files[0] = string(argv[1])  +  "/" + "C2Results_train_pos.txt";
	c2_files[1] = string(argv[1])  +  "/" + "C2Results_train_neg.txt";
	c2_files[2] = string(argv[1])  +  "/" + "C2Results_test_pos.txt";
	c2_files[3] = string(argv[1])  +  "/" + "C2Results_test_neg.txt";

	for ( int i=0 ; i<c2_files.size() ; i++ )
		cout<<c2_files[i]<<endl;

	num_of_patches = atoi ( argv[3] );
	cout<<num_of_patches<<endl;

	fstream fin;
	cerr<<"argv[2]="<<argv[2]<<endl;
	fin.open ( argv[2] , ios::in );
	if ( fin.fail() ){
		cerr<<"file "<<argv[2]<<" can not be opened\n";
		return -1;
	}

	total_size = 0;
	cerr<<"before reading sizes\n";
	for ( int i=0 ; i<4 ; i++ ){
		fin>>sizes[i];

		total_size += sizes[i];
		cout<<"size is "<<sizes[i];
	}
	fin.close();
	cerr<<"after reading sizes\n";

	test_pos = sizes[2];
	test_neg = sizes[3];
	total_tests = test_pos + test_neg;
	final_results = new float [total_tests];

	c2_results = new float *[total_size];
	
	for ( int i=0 ; i<total_size ; i++){
		c2_results[i] = new float [num_of_patches];
	}

	int index=0;
	cout<<"start_reading_files\n";
	for ( int i=0 ; i<4 ; index += sizes[i] , i++ ){
		read_c2_results ( c2_files[i] , c2_results , index , sizes[i] , num_of_patches );
		cerr<<"reading file "<<i<<endl;
	}

	//print_c2_results ( c2_results , sizes , num_of_patches );
	cerr<<"after reading files\n";


	svm_libsvm ( c2_results , sizes , num_of_patches , final_results );

	success_predicts = 0;

	for ( int i=0 ; i<test_pos ; i++ ){
		if ( final_results[i] == 1 ){
			success_predicts ++;
		}
	}
	for ( int i=0 ; i<test_neg ; i++ ){
		if ( final_results[i+test_pos] == -1 ){
			success_predicts ++;
		}
	}

	cout<<"success rate is "<<(double)success_predicts / (double)total_tests<<endl;


	return 0;

}




int read_c2_results ( string c2_file , float **c2_results , int start_index , int size , int num_of_patches ){

	fstream fin;
	fin.open ( c2_file.c_str() , ios::in );
	if ( fin.fail()  ){
		cerr<<"file "<<c2_file<<" can not be opend\n";
		return -1;
	}

	for ( int i=0 ; i<size ; i++ ){

		for ( int j=0 ; j<num_of_patches ; j++ ){

			if (  !(fin>>c2_results[start_index + i][j] )) {
				cerr<<"before completely reading file "<<c2_file<<" we reached end of file\n";
			}
		}

	}

	fin.close();

}


void print_c2_results ( float **c2_results , vector<int> sizes  , int num_of_patches){


	int index = 0;
	for ( int i=0 ; i<sizes.size() ; index += sizes[i], i++){

		for ( int j=0 ; j<sizes[i] ; j++ ){

			for ( int k=0 ; k<num_of_patches ; k++){

				cout << c2_results[j+index][k]<<" ";
			}
			cout<<endl;
		}
	}
}
