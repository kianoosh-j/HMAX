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
void conv2 ( double *m1 , double *m2 , int w1 , int h1 , int w2 , int h2 , double *res );
void so_simple_convol ( Picture &m , Picture &kernel , Picture &result );
void so_simple_convol_same ( Picture &m , Picture &kernel , Picture &result_same );
void init_matrix( Picture& m, int width, int height);
void splat_scalar_to_matrix( Picture &m, float value );

#endif
