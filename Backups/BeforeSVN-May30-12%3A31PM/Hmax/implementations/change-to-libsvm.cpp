#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>

#define ARGN 9
#define DIRN 4


using namespace std;


int main(int argn , char *argv[]){

	if ( argn != ARGN ){

		cerr<<"ERROR: mistake in calling the program;\n\n\tplease call like this:\t";
		return -1;
	}


	int size[DIRN];
	int base_index=1;
	int label[DIRN]={1,-1};
	for ( int i=0 ; i<DIRN ; i++){
		size[i] = atoi ( argv[base_index++] );
	}

	int num_of_patches = atoi ( argv[base_index++] );

	fstream fin;
	fin.open ( argv[base_index++] , ios::in );
	fstream ftrain;
	ftrain.open ( argv[base_index++] , ios::out );
	fstream ftest;
	ftest.open ( argv[base_index++] , ios::out );


	for ( int i=0 ; i<2 ; i++){

		for ( int j=0 ; j<size[i] ; j++){

			ftrain<<label[i];
			for ( int k=0 ; k<num_of_patches ; k++){

				float temp;
				fin>>temp;
				ftrain<<" "<<k+1<<":"<<temp;

			}
			ftrain<<"\n";
		}

	}
	


	fin.close();
	ftrain.close();
	ftest.close();
	return 0;

}
