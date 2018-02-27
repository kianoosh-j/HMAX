#ifndef __GABOR_H__
#define __GABOR_H__

#include "headers.h"

float  	Average ( float ** f , int size);

float *	Gabor( int rot , int rf_size , float div);

int 	init_gabor_filters ( list< list<Picture> > &gabor_filters , const list<int> &gabor_filter_sizes , const list<int> &gabor_filter_rotations , const list<float> &div );

#endif
