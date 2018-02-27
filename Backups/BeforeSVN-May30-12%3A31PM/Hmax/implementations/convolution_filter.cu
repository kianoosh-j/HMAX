#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../headers/headers.h"
#include "../headers/picture_handler.h"

#define RAND 10
#define MAX_SIZE 10000
#define ARGN 6
#define GRID 40
#define BLOCK_DIM 128
#define MAX_SHARED 10000


typedef float Picture_Type;



void init_matrix ( Picture &m , int w , int h ){

	if ( w * h >=0 ){
		m.matrix = (Picture_Type *)malloc ( sizeof(Picture_Type) * w * h );
		m.width = w;
		m.height = h;
	}
	else 
		printf ( "error in init_matrix : invalid w and h\n" );
}


void init_gpu_matrix ( Picture &m , int w , int h ){

	if ( w * h >=0 ){
		cudaMalloc ( &(m.matrix) , sizeof(Picture_Type) * w * h );
		m.width = w;
		m.height = h;
	}
	else 
		printf (  "error in init_matrix : invalid w and h\n" );
}


void print_matrix ( Picture &m , const char *messeage=NULL ){

	if ( messeage != NULL )
		printf ( "%s\n" , messeage );

	int index = 0;
	for ( int i=0 ; i<m.width ; i++ ){
		for ( int j=0 ; j<m.height ;j++ ){

			printf ( "%10.5f ", m.matrix[index++] );
		}
		printf ( "\n" );
	}
}


void create_random_matrix ( Picture &m , int base ){


	for ( int i=0 ; i<m.width *m.height ; i++ ){

		m.matrix[i] = (Picture_Type)(rand() % base);
	}
}


void __global__ ConvolutionSame ( Picture kernel , Picture main_matrix , Picture conv_matrix ){



	// assign each block to compute some rows of conv matrix
	int conv_height = conv_matrix.height + kernel.height - 1;
	int conv_width = conv_matrix.width + kernel.width - 1;
	int rows_per_block = ceil ( ((float) conv_height ) / gridDim.x );
	int first_row = (blockIdx.x) * rows_per_block;


	// getting shared memory as much as enough
	__shared__ Picture_Type shared_memory [MAX_SHARED];


	// getting first part of shared memory for row of each conv matrix
	Picture_Type *conv_row_shared_memory = &(shared_memory[0]);


	// moving kernel to shared memory
	Picture_Type *kernel_shared = &(shared_memory[conv_width]);
	int temp_index = threadIdx.x;
	int kernel_total_elements = kernel.width * kernel.height;
	while ( temp_index < kernel_total_elements ){
		kernel_shared[temp_index] = kernel.matrix[temp_index];
		temp_index += blockDim.x;
	}
	syncthreads();

	// for each row of conv matrix
	int current_row = first_row + ( (kernel.height - 1) / 2 ) ;
	for ( int i=0    ;     i<rows_per_block && current_row < conv_height     ; i++ , current_row++  ){

		// find rang rows of kernel and main matrix that should multiply to each other to do this row of conv matrix
		int kernel_top_bound = (kernel.height -1) - max (kernel.height - current_row -1 , 0 );
		int kernel_bot_bound = (kernel.height -1) - min ( kernel.height-1 , conv_height - current_row - 1  );
		int main_matrix_top_bound = max ( current_row + 1 - kernel.height , 0 );
		int main_matrix_bot_bound = min ( current_row , main_matrix.height -1 );


		// reset the shared memory for this row
		int temp_index = threadIdx.x;
		while ( temp_index < conv_width ){
			conv_row_shared_memory[temp_index] = 0;
			temp_index += blockDim.x;
		}
		syncthreads();


		// for each row in range
		for ( 		int main_row = main_matrix_top_bound , kernel_row = kernel_top_bound 	;	
				main_row <= main_matrix_bot_bound && kernel_row >= kernel_bot_bound	;
				main_row++ , kernel_row--
		    ){

			// set pointers for walking over rows
			Picture_Type *main_row_pointer = &( main_matrix.matrix[ main_row*main_matrix.width ] ) + threadIdx.x; // it points to the first of row + thread Index
			Picture_Type *kernel_row_pointer = &( kernel_shared [ kernel_row * kernel.width ] ); // it points to the begining of row in kernel
			Picture_Type *conv_row_pointer = conv_row_shared_memory + threadIdx.x;

			// mult all elements of both rows of kernel and main matrix
			int main_col = threadIdx.x;
			while ( true ){

				// check if is finished using the thread 0 main_col
				if ( (main_col - threadIdx.x) >= main_matrix.width ){
					break;
				}

				// multiply this col to all columns of kernel row
				int kernel_col = 0;
				Picture_Type main_element_temp = *main_row_pointer;
				while ( kernel_col < kernel.width ){

					if ( main_col < main_matrix.width ){
						*conv_row_pointer += main_element_temp * (*kernel_row_pointer);
					}
					syncthreads();
					kernel_col++;
					kernel_row_pointer++;
					conv_row_pointer++;
				}

				// increment main_col and correct pointers
				main_col += blockDim.x;
				main_row_pointer += blockDim.x;
				kernel_row_pointer += kernel.width;
				conv_row_pointer += ( blockDim.x - kernel.width );
			}
		}
		// end for


		// move this shared memory to the conv matrix in global memory
		temp_index = threadIdx.x + (  kernel.width / 2 );
		Picture_Type *conv_row_pointer = & ( conv_matrix.matrix [ (current_row-1) * conv_matrix.width ] );
		int thread_index = threadIdx.x;
		while ( thread_index < conv_matrix.width  ){

			conv_row_pointer [ thread_index ] = conv_row_shared_memory [ temp_index ];
			temp_index += blockDim.x;
			thread_index += blockDim.x;
		}
		// end moving

	}
	// end for
}


void __global__ Convolution ( Picture kernel , Picture main_matrix , Picture conv_matrix ){



	// assign each block to compute some rows of conv matrix
	//int conv_height = conv_matrix.height + kernel.height - 1;
	//int conv_width = conv_matrix.width + kernel.width - 1;
	int rows_per_block = ceil ( ((float) conv_matrix.height ) / gridDim.x );
	int first_row = (blockIdx.x) * rows_per_block;


	// getting shared memory as much as enough
	__shared__ Picture_Type shared_memory [MAX_SHARED];


	// getting first part of shared memory for row of each conv matrix
	Picture_Type *conv_row_shared_memory = &(shared_memory[0]);


	// moving kernel to shared memory
	Picture_Type *kernel_shared = &(shared_memory[conv_matrix.width]);
	int temp_index = threadIdx.x;
	int kernel_total_elements = kernel.width * kernel.height;
	while ( temp_index < kernel_total_elements ){
		kernel_shared[temp_index] = kernel.matrix[temp_index];
		temp_index += blockDim.x;
	}
	syncthreads();

	// for each row of conv matrix
	int current_row = first_row;
	for ( int i=0    ;     i<rows_per_block && current_row < conv_matrix.height     ; i++ , current_row++  ){

		//if ( threadIdx.x == 0)
		//	printf ( "%d %d\n" , blockIdx.x , current_row );
		// find rang rows of kernel and main matrix that should multiply to each other to do this row of conv matrix
		int kernel_top_bound = (kernel.height -1) - max (kernel.height - current_row -1 , 0 );
		int kernel_bot_bound = (kernel.height -1) - min ( kernel.height-1 , conv_matrix.height - current_row - 1  );
		int main_matrix_top_bound = max ( current_row + 1 - kernel.height , 0 );
		int main_matrix_bot_bound = min ( current_row , main_matrix.height -1 );


		// reset the shared memory for this row
		int temp_index = threadIdx.x;
		while ( temp_index < conv_matrix.width ){
			conv_row_shared_memory[temp_index] = 0;
			temp_index += blockDim.x;
		}
		syncthreads();


		// for each row in range
		for ( 		int main_row = main_matrix_top_bound , kernel_row = kernel_top_bound 	;	
				main_row <= main_matrix_bot_bound && kernel_row >= kernel_bot_bound	;
				main_row++ , kernel_row--
		    ){

			// set pointers for walking over rows
			Picture_Type *main_row_pointer = &( main_matrix.matrix[ main_row*main_matrix.width ] ) + threadIdx.x; // it points to the first of row + thread Index
			Picture_Type *kernel_row_pointer = &( kernel_shared [ kernel_row * kernel.width ] ); // it points to the begining of row in kernel
			Picture_Type *conv_row_pointer = conv_row_shared_memory + threadIdx.x;

			// mult all elements of both rows of kernel and main matrix
			int main_col = threadIdx.x;
			while ( true ){

				// check if is finished using the thread 0 main_col
				if ( (main_col - threadIdx.x) >= main_matrix.width ){
					break;
				}

				// multiply this col to all columns of kernel row
				int kernel_col = 0;
				Picture_Type main_element_temp = *main_row_pointer;
				while ( kernel_col < kernel.width ){

					if ( main_col < main_matrix.width ){
						*conv_row_pointer += main_element_temp * (*kernel_row_pointer);
					}
					syncthreads();
					kernel_col++;
					kernel_row_pointer++;
					conv_row_pointer++;
				}

				// increment main_col and correct pointers
				main_col += blockDim.x;
				main_row_pointer += blockDim.x;
				kernel_row_pointer += kernel.width;
				conv_row_pointer += ( blockDim.x - kernel.width );
			}
		}
		// end for


		// move this shared memory to the conv matrix in global memory
		temp_index = threadIdx.x;
		Picture_Type *conv_row_pointer = & ( conv_matrix.matrix [ current_row * conv_matrix.width ] );
		while ( temp_index < conv_matrix.width  ){

			conv_row_pointer [ temp_index ] = conv_row_shared_memory [ temp_index ];
			temp_index += blockDim.x;
		}
		// end moving

	}
	// end for
}


void __global__ FilterSame ( Picture kernel , Picture main_matrix , Picture conv_matrix ){


	int conv_height = conv_matrix.height + kernel.height - 1;
	int conv_width = conv_matrix.width + kernel.width - 1;
	// assign each block to compute some rows of conv matrix
	int rows_per_block = ceil ( ((float) conv_height ) / gridDim.x );
	int first_row = blockIdx.x * rows_per_block;


	// getting shared memory as much as enough
	__shared__ Picture_Type shared_memory [MAX_SHARED];


	// getting first part of shared memory for row of each conv matrix
	Picture_Type *conv_row_shared_memory = &(shared_memory[0]);


	// moving kernel to shared memory
	Picture_Type *kernel_shared = &(shared_memory[conv_width]);
	int temp_index = threadIdx.x;
	int kernel_total_elements = kernel.width * kernel.height;
	while ( temp_index < kernel_total_elements ){
		kernel_shared[temp_index] = kernel.matrix[temp_index];
		temp_index += blockDim.x;
	}
	syncthreads();

	// for each row of conv matrix
	int current_row = first_row + ( (kernel.height - 1) / 2 ) ;
	for ( int i=0    ;     i<rows_per_block && current_row < conv_height     ; i++ , current_row++  ){

		// find rang rows of kernel and main matrix that should multiply to each other to do this row of conv matrix
		int kernel_top_bound = max (kernel.height - current_row -1 , 0 );
		int kernel_bot_bound = min ( kernel.height-1 , conv_height - current_row - 1  );
		int main_matrix_top_bound = max ( current_row + 1 - kernel.height , 0 );
		int main_matrix_bot_bound = min ( current_row , main_matrix.height -1 );


		// reset the shared memory for this row
		int temp_index = threadIdx.x;
		while ( temp_index < conv_width ){
			conv_row_shared_memory[temp_index] = 0;
			temp_index += blockDim.x;
		}
		syncthreads();

		// for each row in range
		for ( 		int main_row = main_matrix_top_bound , kernel_row = kernel_top_bound 	;	
				main_row <= main_matrix_bot_bound && kernel_row <= kernel_bot_bound	;
				main_row++ , kernel_row++ 
		    ){

			// set pointers for walking over rows
			Picture_Type *main_row_pointer = &( main_matrix.matrix[ main_row*main_matrix.width ] ) + threadIdx.x; // it points to the first of row + thread Index
			Picture_Type *kernel_row_pointer = &( kernel_shared [ (kernel_row+1)*kernel.width -1 ] ); // it points to the end of row in kernel
			Picture_Type *conv_row_pointer = conv_row_shared_memory + threadIdx.x;

			// mult all elements of both rows of kernel and main matrix
			int main_col = threadIdx.x;
			while ( true ){

				// check if is finished using the thread 0 main_col
				if ( (main_col - threadIdx.x) >= main_matrix.width ){
					break;
				}

				// multiply this col to all columns of kernel row
				int kernel_col = kernel.width-1;
				Picture_Type main_element_temp = *main_row_pointer;
				while ( kernel_col >= 0 ){

					if ( main_col < main_matrix.width ){
						*conv_row_pointer += main_element_temp * (*kernel_row_pointer);
					}
					syncthreads();
					kernel_col--;
					kernel_row_pointer--;
					conv_row_pointer++;
				}

				// increment main_col and correct pointers
				main_col += blockDim.x;
				main_row_pointer += blockDim.x;
				kernel_row_pointer += kernel.width;
				conv_row_pointer += blockDim.x - kernel.width;
			}

		}
		// end for

		// move this shared memory to the conv matrix in global memory
		temp_index = threadIdx.x + (  kernel.width / 2 );
		Picture_Type *conv_row_pointer = & ( conv_matrix.matrix [ (current_row-1) * conv_matrix.width ] );
		int thread_index = threadIdx.x;
		while ( thread_index < conv_matrix.width  ){

			conv_row_pointer [ thread_index ] = conv_row_shared_memory [ temp_index ];
			temp_index += blockDim.x;
			thread_index += blockDim.x;
		}






		// end moving

	}
	// end for
}


void __global__ Filter ( Picture kernel , Picture main_matrix , Picture conv_matrix ){


	// assign each block to compute some rows of conv matrix
	int rows_per_block = ceil ( ((float) conv_matrix.height ) / gridDim.x );
	int first_row = blockIdx.x * rows_per_block;


	// getting shared memory as much as enough
	__shared__ Picture_Type shared_memory [MAX_SHARED];


	// getting first part of shared memory for row of each conv matrix
	Picture_Type *conv_row_shared_memory = &(shared_memory[0]);


	// moving kernel to shared memory
	Picture_Type *kernel_shared = &(shared_memory[conv_matrix.width]);
	int temp_index = threadIdx.x;
	int kernel_total_elements = kernel.width * kernel.height;
	while ( temp_index < kernel_total_elements ){
		kernel_shared[temp_index] = kernel.matrix[temp_index];
		temp_index += blockDim.x;
	}
	syncthreads();

	// for each row of conv matrix
	int current_row = first_row ;
	for ( int i=0    ;     i<rows_per_block && current_row < conv_matrix.height     ; i++ , current_row++  ){

		// find rang rows of kernel and main matrix that should multiply to each other to do this row of conv matrix
		int kernel_top_bound = max (kernel.height - current_row -1 , 0 );
		int kernel_bot_bound = min ( kernel.height-1 , conv_matrix.height - current_row - 1  );
		int main_matrix_top_bound = max ( current_row + 1 - kernel.height , 0 );
		int main_matrix_bot_bound = min ( current_row , main_matrix.height -1 );


		// reset the shared memory for this row
		int temp_index = threadIdx.x;
		while ( temp_index < conv_matrix.width ){
			conv_row_shared_memory[temp_index] = 0;
			temp_index += blockDim.x;
		}
		syncthreads();


		// for each row in range
		for ( 		int main_row = main_matrix_top_bound , kernel_row = kernel_top_bound 	;	
				main_row <= main_matrix_bot_bound && kernel_row <= kernel_bot_bound	;
				main_row++ , kernel_row++ 
				){

			// set pointers for walking over rows
			Picture_Type *main_row_pointer = &( main_matrix.matrix[ main_row*main_matrix.width ] ) + threadIdx.x; // it points to the first of row + thread Index
			Picture_Type *kernel_row_pointer = &( kernel_shared [ (kernel_row+1)*kernel.width -1 ] ); // it points to the end of row in kernel
			Picture_Type *conv_row_pointer = conv_row_shared_memory + threadIdx.x;

			// mult all elements of both rows of kernel and main matrix
			int main_col = threadIdx.x;
			while ( true ){

				// check if is finished using the thread 0 main_col
				if ( (main_col - threadIdx.x) >= main_matrix.width ){
					break;
				}

				// multiply this col to all columns of kernel row
				int kernel_col = kernel.width-1;
				Picture_Type main_element_temp = *main_row_pointer;
				while ( kernel_col >= 0 ){

					if ( main_col < main_matrix.width ){
						*conv_row_pointer += main_element_temp * (*kernel_row_pointer);
					}
					syncthreads();
					kernel_col--;
					kernel_row_pointer--;
					conv_row_pointer++;
				}

				// increment main_col and correct pointers
				main_col += blockDim.x;
				main_row_pointer += blockDim.x;
				kernel_row_pointer += kernel.width;
				conv_row_pointer += blockDim.x - kernel.width;
			}

		}
		// end for


		// move this shared memory to the conv matrix in global memory
		temp_index = threadIdx.x;
		Picture_Type *conv_row_pointer = & ( conv_matrix.matrix [ current_row * conv_matrix.width ] );
		while ( temp_index < conv_matrix.width ){

			conv_row_pointer [ temp_index ] = conv_row_shared_memory [ temp_index ];
			temp_index += blockDim.x;
		}
		// end moving

	}
	// end for
}


int imfilter_pictures_same_gpu ( Picture &p1 , Picture &p2 , Picture &pres ){

	if ( !valid_picture (p2) || !equal_size_pictures (p1,pres) ){
		cerr<<"error : conv2_pictures_same : size of pictures not OK\n";
		return -1;
	}

	Picture dev_p1,dev_p2,dev_pres;

	dev_p1.width = p1.width;
	dev_p1.height = p1.height;

	dev_p2.width = p1.width;
	dev_p2.height = p2.height;

	dev_pres.width = pres.width;
	dev_pres.height = pres.height;


	cudaMalloc((void**)&dev_p1.matrix,sizeof(float)*p1.width*p1.height );
	cudaMalloc((void**)&dev_p2.matrix,sizeof(float)*p2.width*p2.height );
	cudaMalloc((void**)&dev_pres.matrix,sizeof(float)*pres.width*pres.height );
	
	cudaMemcpy(dev_p1.matrix,p1.matrix,p1.width*p1.height*sizeof(float),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_p2.matrix,p2.matrix,p2.width*p2.height*sizeof(float),cudaMemcpyHostToDevice);

	//FilterSame<<<p1.height/4, p1.width/4>>> (dev_p2,dev_p1,dev_pres);

	cudaMemcpy(pres.matrix,dev_pres.matrix,p1.width*p1.height*sizeof(float),cudaMemcpyDeviceToHost);

	cudaFree (dev_p1.matrix );
	cudaFree (dev_p2.matrix );
	cudaFree (dev_pres.matrix);

	return 0;
}


int conv2_pictures_same_gpu ( Picture &p1 , Picture &p2 , Picture &pres ){


	if ( !valid_picture ( p2 ) || !equal_size_pictures ( p1 , pres ) ){
		cerr<<"error : conv2_pictures_same : size of pictures not OK\n";
		return -1;
	}

	Picture dev_p1,dev_p2,dev_pres;

	dev_p1.width = p1.width;
	dev_p1.height = p1.height;

	dev_p2.width = p1.width;
	dev_p2.height = p2.height;

	dev_pres.width = pres.width;
	dev_pres.height = pres.height;


	cudaMalloc((void**)&dev_p1.matrix,sizeof(float)*p1.width*p1.height );
	cudaMalloc((void**)&dev_p2.matrix,sizeof(float)*p2.width*p2.height );
	cudaMalloc((void**)&dev_pres.matrix,sizeof(float)*pres.width*pres.height );

	cudaMemcpy(dev_p1.matrix,p1.matrix,p1.width*p1.height*sizeof(float),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_p2.matrix,p2.matrix,p2.width*p2.height*sizeof(float),cudaMemcpyHostToDevice);

	//ConvolutionSame<<<p1.height/4,p1.width/4>>> ( dev_p2 , dev_p1 , dev_pres ); 

	cudaMemcpy(pres.matrix,dev_pres.matrix,pres.width*pres.height*sizeof(float),cudaMemcpyDeviceToHost);


	cudaFree (dev_p1.matrix );
	cudaFree (dev_p2.matrix );
	cudaFree (dev_pres.matrix);

	return 0;

}




