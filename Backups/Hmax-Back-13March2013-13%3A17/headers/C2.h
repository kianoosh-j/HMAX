#ifndef __C2__
#define __C2__

#include "headers.h"

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


#endif
