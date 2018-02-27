#include "../headers/headers.h"
#include "../headers/picture_handler.h"
#include "../headers/gabor.h"

float Average (float** f,int size){
	float sum=0;
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			sum += f[i][j];
		}
	}
	float average = sum/(size*size);
	return average;	
}

float* Gabor(int rot, int rf_size,float div){
	float* ans;
	ans=new float[rf_size*rf_size];
	float lambda = rf_size*2/div;
	float sigma  = lambda*0.8;
	float G      = 0.3;
	float theta  = rot*M_PI/180;
	int center = rf_size/2+1;
	int filtSizeL = center-1;
    	int filtSizeR = rf_size-filtSizeL-1;
	float sigmaq  = pow((float)sigma,2);
	float E;
	float sum;
	float x;
	float** f;
	f = new float*[rf_size];
	for(int i=0; i<rf_size; i++){
		f[i] = new float [rf_size];  
	}
	for(int i=-filtSizeL; i<=filtSizeR; i++){
		for(int j=-filtSizeL; j<=filtSizeR; j++){
			if ( sqrt(pow((float)i,2)+pow((float)j,2))>(float)rf_size/2 ){
				E = 0;
			}
			else{
                    		float x = i*cos(theta) - j*sin(theta);
                    		float y = i*sin(theta) + j*cos(theta);
                    		E = exp(-(pow((float)x,2)+pow((float)G,2)*pow((float)y,2))/(2*sigmaq))*cos(2*M_PI*x/lambda);
			}
   
                	f[j+center-1][i+center-1] = E;
		}
	}
	

	float average = Average(f,rf_size);
	for(int i=0; i<rf_size; i++){
		for(int j=0; j<rf_size; j++){
			 f[i][j] = f[i][j] - average;
		}
	}
	
	float sum_f = 0;
	for(int i=0; i<rf_size; i++){
		for(int j=0; j<rf_size; j++){
			sum_f += f[i][j] * f[i][j] ;
		}
	}
	float temp = (float)sqrt(sum_f);
	for(int i=0; i<rf_size; i++){
		for(int j=0; j<rf_size; j++){
			f[i][j] = f[i][j] / temp ;
		}
	}
	
	for(int i=0; i<rf_size; i++){
		for(int j=0; j<rf_size; j++){
			ans[i*rf_size+j] = f[i][j]  ;
		}
	}

	for(int i=0; i<rf_size; i++){
		delete [] f[i];
	}
	delete [] f;
	
	return ans;

}

int init_gabor_filters ( list< list<Picture> > &gabor_filters , const list<int> &gabor_filter_sizes , const list<int> &gabor_filter_rotations , const list<float> &div ){

	list<int>::const_iterator iter_int1, iter_int2;
	list<float>::const_iterator iter_float1;
	for ( iter_int2 = gabor_filter_sizes.begin() , iter_float1 = div.begin() ; iter_float1 != div.end() && iter_int2 != gabor_filter_sizes.end() ; iter_float1++ , iter_int2++){
		list<Picture> temp_filters;
		for ( iter_int1 = gabor_filter_rotations.begin() ; iter_int1 != gabor_filter_rotations.end() ; iter_int1++){
			float *temp_float = Gabor (*iter_int1 , *iter_int2 , *iter_float1 );
			Picture pic;
			pic.width = pic.height = *iter_int2;
			pic.matrix = temp_float;
			inverse_picture ( pic );
			temp_filters.push_back (pic);
			// print_picture ( pic );
			// printf ( "\n\nprinting the gabor filter with size   %d and rotation  %d\n" , *iter_int2 , *iter_int1 );
			// TODO print the gabor filter here to be sure
		}
		gabor_filters.push_back ( temp_filters );
		temp_filters.clear();
	}
	return 0;//TODO
}

void init_gabor_C1(list<int >&gabor_filter_sizes,list<int >&gabor_filter_rotations, list<float >&div){
	gabor_filter_sizes.push_back(11);
	gabor_filter_sizes.push_back(13);
	gabor_filter_rotations.push_back(90);
	gabor_filter_rotations.push_back(-45);
	gabor_filter_rotations.push_back(0);
	gabor_filter_rotations.push_back(45);
	div.push_back(3.9);
	div.push_back(3.85);


}
