#ifndef __MYCONV__
#define __MYCONV__

#include "headers.h"

int 	opencvconvolution(float *m1,float *m2,int width1,int height1,int width2,int height2,float *result);
int 	conv2_pictures_same ( Picture &p1 , Picture &p2 , Picture &pres );
int 	conv2_pictures_same2 ( Picture &p1 , Picture &p2 , Picture &pres );
void 	conv2_same ( float *m1 , float *m2 , int w1 , int h1 , int w2 , int h2 , float *res );
void 	conv2 ( float *m1 , float *m2 , int w1 , int h1 , int w2 , int h2 , float *res );
int 	imfilter_pictures_same ( Picture &p1 , Picture &p2 , Picture &pres );
int 	imfilter(float *a,float *k,int h1,int w1,int h2,int w2,float *result );

#endif
