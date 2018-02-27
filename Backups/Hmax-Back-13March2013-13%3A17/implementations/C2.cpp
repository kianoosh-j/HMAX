#include "../headers/C2.h"
#include "../headers/C1.h"
#include "../headers/myconv.h"
#include "../headers/headers.h"
#include "../headers/gabor.h"
#include "../headers/picture_handler.h"

#define NumOfThreads 1


void *C2_function(void* In){
	input_data* input = (input_data*) In; 
	int count = 0;
	int dim1,dim2,dim3;
	dim1=0;
	for(list<list<Picture> >::iterator iter1 = input->pictures->begin(); iter1 != input->pictures->end(); iter1++){
		dim2=0;
		for(list<Picture>::iterator iter2 = iter1->begin(); iter2 != iter1->end(); iter2++){
			dim3=0;
			if( count % NumOfThreads == input->threadID ){

				if(C1_Function(*iter2,*(input->S1_pictures), *(input->C1_result), input->gabor_filters , input->c1SpaceSS, input->c1ScaleSS, input->c10L) == -1 ){
					input->output = -1;
					pthread_exit(0);
				}
				float min;
				for(list<list<list<Picture> > > ::iterator it1 = input->patches->begin();it1 != input->patches->end(); it1++){
					for(list<list<Picture> >::iterator it2 = (*it1).begin(); it2 != (*it1).end(); it2++){
						min = 1000000000;
						for(list<list<Picture> >::iterator PicIter = input->C1_result->begin(); PicIter != input->C1_result->end(); PicIter++ ){
							Picture distance;
							window_patch_distance(*PicIter,*it2, distance);
							float tmp = Min(distance);
							free_picture ( distance );
							min = tmp < min ? tmp : min;

						}

					        (*input->FinalResult)[dim1][dim2][dim3] = min;
						dim3++;
					}
				}
				free_list_list_pictures(*(input->C1_result));
				free_list_list_pictures(*(input->S1_pictures));
			}
			count++;
			dim2++;
		}
		dim1++;
	}
	input->output = 0;
	pthread_exit(0);
}

float Min(Picture pic){

	float min = 1000000000000;
	for(int i = 0; i < pic.height * pic.width; i++)
		if(pic.matrix[i] < min)
			min = pic.matrix[i];	



	return min;

}

int run_c2_for_all_pictures ( list<list<Picture> >&pictures , list<list<list<Picture> > >&patches , vector<vector<vector<float> > > &C2_result){


	input_data* input;
	input = new input_data[NumOfThreads];
	//int num_of_patches = 250 * 4;//TODO it should be counted following a formula

	//allocate_c2_result (  pictures , C2_result ,  num_of_patches );



	list<list<Picture> > S1_pictures[NumOfThreads];
	list<list<Picture> > C1_result[NumOfThreads];

	if ( pictures.size() != 4 ){
		cerr<<"error : run_c2_for_all_pictures : size of pictures should be 4 ; train_pos, train_neg, test_pos, test_neg\n";
		return -1;
	}

	list<list<Picture> >::iterator iter_lpicture;
	list<Picture>::iterator iter_picture;



	pthread_t* thread;
	thread = new pthread_t [NumOfThreads];

	for(int g=0; g < NumOfThreads; g++){


		init_c2_parameters ( input[g].c1SpaceSS , input[g].c1ScaleSS , input[g].c10L , input[g].gabor_filters );


		input[g].threadID = g;
		input[g].pictures = &pictures;

		input[g].S1_pictures = S1_pictures+g;

		input[g].C1_result = C1_result+g;


		//input[g].C2_result = C2_result_strc + g;

		input[g].patches = &patches;

		input[g].output = 0;
		input[g].FinalResult = &C2_result;


		pthread_create(&thread[g], NULL, C2_function, (void*) (input+g));
		


	}
	for(int g=0; g < NumOfThreads; g++){
		pthread_join(thread[g],NULL);
		cout << "end of thread[" << g << "] " << endl;
		if(input[g].output == -1)
			return -1;

	}

	/*
		cout << "########################################" << endl;
		cout << "printing C2_result at the end of run_C2_for_all_pictures  " << endl;
		for(vector<vector<vector<float> > >::iterator iter1 = C2_result.begin(); iter1 != C2_result.end(); iter1++){
			for(vector<vector<float> >::iterator iter2 = iter1->begin(); iter2 != iter1->end(); iter2++){
				for(vector<float>::iterator iter3 = iter2->begin(); iter3 != iter2->end(); iter3++)
					cout << *iter3 << "\t" ;
			}
			cout << endl;
		}

		cout << "########################################" << endl;

*/



return 0;

}

void init_c2_parameters ( vector<int> &c1SpaceSS, vector<int> &c1ScaleSS, int &c10L, list<list<Picture> >&gabor_filters ){


	c1SpaceSS.resize(0);
	c1ScaleSS.resize(0);
	list<float> Div;
	list<int> rotations;
	list<int> gabor_filter_sizes;

	int minSpaceSS=8;
	int maxSpaceSS=22;
	int SpaceSS_step=2;
	for ( int i=minSpaceSS ; i<=maxSpaceSS ; i+=SpaceSS_step ){
		c1SpaceSS.push_back(i);
	}

	int minScaleSS=1;
	int maxScaleSS=16;
	int ScaleSS_step=2;
	for ( int i=minScaleSS ; i<=maxScaleSS ; i+=ScaleSS_step ){
		c1ScaleSS.push_back(i);
	}

	c10L=2;//as in the matlab code

	float minDiv=3.2;
	float maxDiv=4;
	float Div_step = -0.05;
	for ( float i=maxDiv ; i>=minDiv ; i+=Div_step ){
		Div.push_back ( i );
	}

	rotations.push_back(90);
	rotations.push_back(-45);
	rotations.push_back(0);
	rotations.push_back(45);

	int minFS=7;
	int maxFS=37;
	int FS_step=2;
	for ( int i=minFS ; i<= maxFS ; i+=FS_step ){
		gabor_filter_sizes.push_back (i);
	}

	init_gabor_filters ( gabor_filters , gabor_filter_sizes , rotations , Div );

}

/*

int allocate_c2_result ( list<list<Picture> >&pictures , vector<vector<vector<float> > > &C2_result , int num_of_patches){

	C2_result.resize ( pictures.size() );
	int index=0;

	for ( list<list<Picture> >::iterator iter_lpicture = pictures.begin() ; iter_lpicture != pictures.end() ; iter_lpicture++){
		C2_result[index].resize ( iter_lpicture->size() );
		for ( int i=0 ; i<C2_result[index].size() ; i++){
			C2_result[index][i].resize ( num_of_patches );
		}
		index++;
	}
	return 0;

}
*/

/*
void InsertInVector(vector<vector<vector<float> > >* pictures, int num, int dim1, int dim2, int dim3){

	//for(vector< vector<vector<float> > >::iterator iter1 = pictures.begin(); iter1 != pictures.end(); iter1++)
	
	for(int i=0; i < dim1; i++)
		
	int dimension1 = dim1 * pictures.size();
	int dimension2 = pictures[dim1].size() * dim2;
	int dimension3 = pictures[dim1][dim2].size() * dim3;
	pictures + dimension1 + dimension2 + dimension3 = num;

}
*/

int allocate_c2_result ( list<list<Picture> >&pictures , vector<vector<vector<float> > > &C2_result , int num_of_patches){

	C2_result.resize ( pictures.size() );
	int index=0;

	for ( list<list<Picture> >::iterator iter_lpicture = pictures.begin() ; iter_lpicture != pictures.end() ; iter_lpicture++){
		C2_result[index].resize ( iter_lpicture->size() );
		for ( int i=0 ; i<C2_result[index].size() ; i++){
			C2_result[index][i].resize ( num_of_patches );
		}
		index++;
	}
	return 0;

}

int window_patch_distance(list<Picture> &pictures, list<Picture> & patches,Picture  &distance){


	//temporary commenting !!!
	/*
	if(pictures.size() != 4 || patches.size() != 4){
		cerr << "WindowPatchDistance :: The list doesn't contain 4 elements!!! " << endl;
		return -1;
	}
	*/

	list<Picture>::iterator iter2 = ++pictures.begin();

	for(list<Picture>::iterator iter = pictures.begin(); iter != --pictures.end(); iter++ ){
		if(iter-> height != iter2->height){
			cerr << "WindowPatchDistance::The height of the elements in the list of entered  pictures don't match!!!" <<endl;
			return -1;
		}
		if(iter->width != iter2->width){
			cerr << "WindowPatchDistance::The width of the elements in the list of entered pictures don't match!!! " << endl;
			return -1;
		}
		iter2++;
	}
	iter2 = patches.begin();
	iter2++;

	for(list<Picture>::iterator iter = patches.begin(); iter != --patches.end(); iter++){
		if(iter -> height != iter2->height){
			cerr << "WindowPatchDistance::The height of the elements in the list of entered patches don't match!!!" <<endl;
			return -1;
		}
		if(iter -> width != iter2->width){
			cerr << "WindowPatchDistance::The width of the elements in the list of entered patches don't match!!!" <<endl;
			return -1;
		}
		iter2++;	
	}

	float PatchSum = 0;
	for(list<Picture>::iterator iter = patches.begin(); iter != patches.end(); iter++){

		for(int i=0;i < iter->width * iter->height; i++){
			PatchSum += iter->matrix[i] * iter->matrix[i];
			//		cout << iter->matrix[i] << endl;
		}
		//	cout << endl;	
	}
	//cout<<"Mahmoud Patch Sum is "<<PatchSum<<endl;

	//with the assumption that all the input pictures have the same size

	int ImgSize = pictures.begin()->width * pictures.begin()->height;
	float* ImgSum = new float[ImgSize];

	//cout << "ImgSUM" << endl;
	for(int i=0; i < ImgSize ; i++ ){
		//	cout << pictures.begin()->matrix[i] << "  " ;
		ImgSum[i] = 0;
		for(list<Picture>::iterator iter = pictures.begin(); iter != pictures.end(); iter++){

			ImgSum[i] += (iter->matrix[i] * iter->matrix[i]);
			//cout << ImgSum[i]<< "  ";
		}
	}
	Picture Imsq;
	Imsq.matrix = ImgSum;
	Imsq.width = pictures.begin()->width;
	Imsq.height = pictures.begin()->height;
	//cout << "Image Summmmmmm" << endl;
	//for(int i=0;i < ImgSize;i++)
	//cout << ImgSum[i] << "  ";
	//cout << endl;
	float* PI;

	int OutputSize = pictures.begin()->height * pictures.begin()->width;
	PI = new float[OutputSize];
	//	cout << "ImgSum:::::::::::::::::::::::::::; " << endl;
	//for(int i=0;i < ImgSize;i++){

	//	if(i % pictures.begin()->width == 0)
	//		cout << endl;
	//		cout << matrix[i] << "  ";
	//}
	int sum_support[4] = { ceil(patches.begin()->width/2.0)-1 , ceil(patches.begin()->height/2.0)-1 , floor(patches.begin()->width/2.0) , floor(patches.begin()->height/2.0) };
	//SumFilter(ImgSum,pictures.begin()->width,pictures.begin()->height,patches.begin()->width/2);
	SumFilter(Imsq, sum_support , 4 );
	/*	cout << "Result of SumFilter  " << endl;
		for(int i=0; i < OutputSize;i++)
		cout << ImgSum[i] << "  ";
		*/
	//cout<<"Mahmoud Imsq\n";
	//print_picture ( Imsq );
	for(int i=0; i < OutputSize;i++)
		PI[i]=0;
	/*


	   Picture pic_temp;
	   pic_temp.matrix = PI;
	   pic_temp.width = pictures.begin()->width;
	   pic_temp.height = pictures.begin()->height;
	   */

	Picture pic_temp2;
	Picture PI_pic;
	init_picture ( pic_temp2 , pictures.begin()->width, pictures.begin()->height );
	PI_pic.width = pic_temp2.width;
	PI_pic.height = pic_temp2.height;
	PI_pic.matrix=PI;

	iter2 = patches.begin();
	for(list<Picture>::iterator iter = pictures.begin(); iter != pictures.end(); iter++ ){
		//with the assumption that the picture and the patches are square matrixes

		conv2_pictures_same ( *iter , *iter2 , pic_temp2 );
		//cout << "Printing picture for CONV" << endl << "$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
		//print_picture(*iter);

		//cout << "Printing patch for CONV" << endl << "$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
		//print_picture(*iter2);
		//cout << "Printing result for CONV" << endl << "$$$$$$$$$$$$$$$$$$$$$$$$" << endl;

		//cout << "END OF printing result for CONV" << endl << "$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
		ADD(PI,pic_temp2.matrix,PI,OutputSize);
		//cout << "PIIIIIIIIIIIIIIIIIIIIII" << endl;
		//print_matrix(PI,1,OutputSize);
		//cout<<"\n\nMahmoud PI PI\n\n";
		//print_picture(PI_pic);
		//cout << "IIIIIIIIIIIIIIIIIIIIIII" << endl;
		iter2++;	
		//	free_picture(pic_temp2);

		//	cout << "HERE IS PI" << endl;
		//	for(int i=0;i< OutputSize;i++)
		//		cout << PI[i] << " ";
	}

	free_picture ( pic_temp2 );

	init_picture ( distance , PI_pic.width , PI_pic.height );

	//float* D = new float[OutputSize];
	//float * I;// = ScalarMUL(PI,-2,OutputSize );
	//I = ScalarAdd(ScalarMUL(PI,-2,OutputSize),PatchSum,OutputSize);
	//cout << "LEILI  " <<endl<< distance.matrix[0]<< "*";
	//cout << "CHECKIIIIING" << endl;
	//cout << PI[0] << endl;
	ScalarMUL(PI,-2,distance.matrix,OutputSize);
	//cout<<"\n\nMahmoud Distance -2 * PI\n\n";
	//print_picture(distance);
	//cout << distance.matrix[0] << endl;
	//cout << distance.matrix[0] << "*";
	ADD(distance.matrix,ImgSum,distance.matrix,OutputSize);
	//cout<<"\n\nMahmoud Distance Imsq\n\n";
	//print_picture(distance);
	ScalarAdd(distance.matrix,PatchSum,distance.matrix,OutputSize);
	//cout<<"\n\nMahmoud Distance ADD\n\n";
	//print_picture(distance);
	//cout << distance.matrix[0] << "*";

	//cout << distance.matrix[0] <<"patch SUM::" << PatchSum <<  endl;

	//cout << distance.matrix[0] << endl;
	//cout << distance.matrix[0] << "*";
	//ADD(ScalarAdd(ScalarMUL(PI,-2,OutputSize),PatchSum,OutputSize),ImgSum,OutputSize);
	//cout << "Here Is IMSQ " << endl;	
	//for(int i=0; i < OutputSize;i++)
	//cout << ImgSum[i] << " "; 


	distance.height = pictures.begin()->height;
	distance.width = pictures.begin()->width;

	delete [] PI;
	delete [] ImgSum;
	return 0;
}

void ADD(float* A,float* B,float* result,int size){


	for(int i=0; i < size; i++)
		result[i] = A[i] + B[i];

}

void ScalarMUL(float* A,float r,float* result,int size){


	for(int i=0; i<size; i++){
		result[i] = A[i] * r;	
	}

}

void ScalarAdd(float* A,float r,float* result,int size){

	for(int i=0; i < size; i++)
		result[i] = A[i] + r;
}



