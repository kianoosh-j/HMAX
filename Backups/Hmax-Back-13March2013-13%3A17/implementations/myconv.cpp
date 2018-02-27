#include "../headers/picture_handler.h"
#include "../headers/myconv.h"
#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
using namespace cv;


void conv2 ( float *m1 , float *m2 , int w1 , int h1 , int w2 , int h2 , float *res ){

	cerr<<"conv2 : start\n";
	int w3 = w2 + w1 -1;
	int h3 = h2 + h1 -1;

	for ( int i=0 ; i<h3 ; i++ ){
		for ( int j=0 ; j<w3 ;j++ ){

			int temp = 0;

			for ( int k = max ( 0 , i-h2+1 ) ; k <= min (i , h1-1) ; k++ ){
				for ( int l=max ( 0 , j-w2+1) ; l <= min ( j  , w1-1) ; l++ ){

					int index1 = k*w1+l;
					int index2 = ((i-k)*w2 + (j-l) );
					temp += m1[index1] * m2[index2];
					//cout<<m1[index1]<<"*"<<m2[index2]<<endl;
					//cout<<index1<<"*"<<index2<<endl;
				}
			}

			res[i*w3+j] = temp;

		}
	}
	cerr<<"conv2 : end\n";

}

void conv2_same ( float *m1 , float *m2 , int w1 , int h1 , int w2 , int h2 , float *res ){

	cerr<<"conv2_same : start\n";
	int wtemp=w1+w2-1;
	int htemp=h1+h2-1;

	float *temp_res = new float [wtemp*htemp];

	conv2 ( m1 , m2 , w1 , h1 , w2 , h2 , temp_res );

	int offset_w = floor ( (float)(w2)/2 );
	int offset_h = floor ( (float)(h2)/2 );
	for ( int i=0 ; i<h1 ; i++ ){
		for ( int j=0 ; j<w1 ; j++){
			int index  = (i+offset_h)*wtemp + ( j+offset_w );
			res[i*w1+j] =  temp_res[index];

		}
	}

	delete [] temp_res;
	cerr<<"conv2_same : end\n";

}

int convol(float *a,float *k,int h1,int w1,int h2,int w2,float *result ){
	Mat img1=Mat(h1,w1,CV_32FC1,a);
	Mat kernel=Mat(h2,w2,CV_32FC1,k);
	Mat img2=Mat(h1,w1,CV_32FC1);
	Point p;

	if(w2%2==0)
	{
		p.x=w2/2-1;
		p.y=w2/2-1;
	}
	else{
		p.x=-1;
		p.y=-1;
	}

	Mat flipped_kernel;
	flip ( kernel , flipped_kernel , -1 );


	filter2D(img1,img2,-1,flipped_kernel,p,0,BORDER_CONSTANT);
	img1.release();
	kernel.release();
	flipped_kernel.release();

	memcpy(result,img2.data,h1*w1*sizeof(float));
	img2.release();
	return 0;
}

int imfilter(float *a,float *k,int h1,int w1,int h2,int w2,float *result ){

	Mat img1=Mat(h1,w1,CV_32FC1,a);
	Mat kernel=Mat(h2,w2,CV_32FC1,k);
	Mat img2=Mat(h1,w1,CV_32FC1);
	Point p;

	if(w2%2==0)
	{
		p.x=w2/2-1;
		p.y=w2/2-1;
	}
	else{
		p.x=-1;
		p.y=-1;
	}


	filter2D(img1,img2,-1,kernel,p,0,BORDER_CONSTANT);
	img1.release();
	kernel.release();

	memcpy(result,img2.data,h1*w1*sizeof(float));
	img2.release();
	return 0;
}

int conv2_pictures_same ( Picture &p1 , Picture &p2 , Picture &pres ){
	

	if ( !valid_picture ( p2 ) || !equal_size_pictures ( p1 , pres ) ){
		cerr<<"error : conv2_pictures_same : size of pictures not OK\n";
		return -1;
	}

	if(convol(p1.matrix,p2.matrix,p1.height,p1.width,p2.height,p2.width,pres.matrix)!= 0 ) {
		cerr<<"error: conv2_pictures_same : convol"<<endl;
		return -1;
	}
	//Picture temp;

	//so_simple_convol_same ( p1 , p2 , pres );
	
	
	return 0;

}

int conv2_pictures_same2 ( Picture &p1 , Picture &p2 , Picture &pres ){


	if ( !valid_picture ( p2 ) || !equal_size_pictures ( p1 , pres ) ){
		cerr<<"error : conv2_pictures_same : size of pictures not OK\n";
		return -1;
	}

	if(convol(p1.matrix,p2.matrix,p1.height,p1.width,p2.height,p2.width,pres.matrix)!= 0 ) {
		cerr<<"error: conv2_pictures_same : convol"<<endl;
		return -1;
	}

	
	return 0;

}

int imfilter_pictures_same ( Picture &p1 , Picture &p2 , Picture &pres ){


	if ( !valid_picture ( p2 ) || !equal_size_pictures ( p1 , pres ) ){
		cerr<<"error : conv2_pictures_same : size of pictures not OK\n";
		return -1;
	}

	if(imfilter(p1.matrix,p2.matrix,p1.height,p1.width,p2.height,p2.width,pres.matrix)!= 0 ) {
		cerr<<"error: conv2_pictures_same : convol"<<endl;
		return -1;
	}


	return 0;

}

void so_simple_convol ( Picture &m , Picture &kernel , Picture &result ){

	//init_matrix ( result , m.width + kernel.width -1 , m.height + kernel.height -1 );
	//splat_scalar_to_matrix ( result , 0.0 );

	conv2 ( &((m.matrix)[0]) , &(kernel.matrix[0]) , m.width , m.height , kernel.width , kernel.height , &(result.matrix[0]) );

}

void so_simple_convol_same ( Picture &m , Picture &kernel , Picture &result_same ){
	
	Picture result;
	init_matrix ( result , m.width + kernel.width -1 , m.height + kernel.height -1 );
	splat_scalar_to_matrix ( result , 0.0 );
	
	so_simple_convol(m, kernel, result);
	int height_start = kernel.height - 1;
	int width_start = kernel.width - 1;
	for(int i=0; i < m.height; i++){
		for(int j=0; j < m.width; j++){
			result_same.matrix[i * m.width + j] = result.matrix[ (i + height_start) * result.width + ( j + width_start ) ];
		}
	}

}
void conv2 ( double *m1 , double *m2 , int w1 , int h1 , int w2 , int h2 , double *res ){

	//cerr<<"conv2 : start\n";
	int w3 = w2 + w1 -1;
	int h3 = h2 + h1 -1;

	for ( int i=0 ; i<h3 ; i++ ){
		for ( int j=0 ; j<w3 ;j++ ){

			int temp = 0;

			for ( int k = max ( 0 , i-h2+1 ) ; k <= min (i , h1-1) ; k++ ){
				for ( int l=max ( 0 , j-w2+1) ; l <= min ( j  , w1-1) ; l++ ){

					int index1 = k*w1+l;
					int index2 = ((i-k)*w2 + (j-l) );
					temp += m1[index1] * m2[index2];
					//cout<<m1[index1]<<"*"<<m2[index2]<<endl;
					//cout<<index1<<"*"<<index2<<endl;
				}
			}

			res[i*w3+j] = temp;

		}
	}
	//cerr<<"conv2 : end\n";

}

void init_matrix( Picture& m, int width, int height){

	m.width = width;
	m.height = height;
	m.matrix = new float [ m.width * m.height ];

}

void splat_scalar_to_matrix( Picture &m, float value ){

	for( int i=0; i < m.width * m.height; i++ )
		m.matrix[i] = value;

}
