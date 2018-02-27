#ifndef __C2__
#define __C2__

#include "headers.h"
#include "Picture.h"

struct input_data{

	int threadID;
	int TempFinalResult;
	list< list < Picture> >* pictures;
	list<list<Picture> >* S1_pictures;
	list<list<Picture> >* C1_result ;
	list<list<list<Picture> > >* patches;
	list<list<Picture> >  gabor_filters;
	vector<int> c1SpaceSS;
	vector<int> c1ScaleSS;
	int c10L;
	int output;
	vector<vector<vector<float> > >* FinalResult;

};


// pictures is a list of 4 pictures including 4 different rotation [0 45 90 135] and patches is also 4 patches in 4 rotations [0 45 90 135]
// return value should be int indicating if it has done ok or no. 0 for ok and -1 if any problem has occured
// Distance should be put in min_distance
void *	C2_function( void * );

float 	Min( Picture pic );

int 	window_patch_distance( list<Picture> &pictures, list<Picture> & patches,Picture  &distance );

void 	ADD( float * A , float * B , float * C , int size);

void 	ScalarMUL( float * A , float r , float * C , int size);

void 	ScalarAdd( float * A , float r , float * , int size);

int 	run_c2_for_all_pictures ( list<list<Picture> >&pictures , list<list<list<Picture> > >&patches , vector<vector<vector<float> > > &C2_result );

void 	init_c2_parameters ( vector<int> &c1SpaceSS, vector<int> &c1ScaleSS, int &c10L, list<list<Picture> >&gabor_filters );

int 	allocate_c2_result ( list<list<Picture> >&pictures , vector<vector<vector<float> > > &C2_result , int num_of_patches);



// it converts c2 results
// source format is vecotr<vector<vector<float> > >
// 	1- 4 types of pictures : train_pos , train_neg, test_pos, test_neg
// 	2- each type has some pictures
// 	3- each picture has some c2 results for each patch
// destination format is float **
// 	1- first dimention for all pictures
// 	2- second dimention for C2's and patches
//	it needs pictures for this purpose
int convert_c2_results_format_from___vector_vector_vecotr_float___TO___2Dfloat_array ( list<list<Picture> > &pictures , vector<vector<vector<float> > > &C2_results , float ** &C2_results_fl );

// print C2 results
void print_C2_results ( vector<vector<vector<float> > > &C2_results );

#endif
