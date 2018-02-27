#include "image_processing.h"
#include "C2.h"
#include "headers.h"
#include "gabor.h"

#define NumOfThreads 7

struct input_data{

	int threadID;
	list< list < Picture> >* pictures;
	list<list<Picture> >* S1_pictures;
	list<list<Picture> >* C1_result ;
	list<list<list<Picture> > >* patches;
	list<list<Picture> >  gabor_filters;
	vector<int> c1SpaceSS;
	vector<int> c1ScaleSS;
	int c10L;
	int output;
//	list< float >*C2_result;
	vector<vector<vector<float> > >* FinalResult;


};


void *C2_function(void* In){
	input_data* input = (input_data*) In; 
	int count = 0;
	int dim1,dim2,dim3;
	cerr<<"start of threadID is "<<input->threadID<<"\t"<<input->pictures->size()<<endl;
	int threadID = input->threadID;
	dim1=0;
	cout << "size of the list of pictures  " << input->pictures->size() << endl;
	for(list<list<Picture> >::iterator iter1 = input->pictures->begin(); iter1 != input->pictures->end(); iter1++){
		dim2=0;
		cout << "size of the inner dimension of pictures " <<threadID<<"\t"<< iter1->size() << endl;
		for(list<Picture>::iterator iter2 = iter1->begin(); iter2 != iter1->end(); iter2++){
			dim3=0;
			if( count % NumOfThreads == input->threadID ){

				cerr << "**********************************************************************************************************"<<endl;
				cerr << "BEFORE CALLING C1 FUNCTION!!!"<<endl;	
				cerr << "sizes are " << input->S1_pictures->size() << "\t" << input->C1_result->size() << "\n";
				cerr << "**********************************************************************************************************"<<endl;

				cout << "printing what the S1 contains;;;BEFORE" << endl;

				if(C1_Function(*iter2,*(input->S1_pictures), *(input->C1_result), input->gabor_filters , input->c1SpaceSS, input->c1ScaleSS, input->c10L) == -1 ){
					cerr <<"C2::error in C1 function!!\t"<<count<< endl;
					input->output = -1;
					cerr<<"end of threadID is "<<input->threadID<<endl;
					pthread_exit(0);
				}
				cerr << "**********************************************************************************************************"<<endl;
				cerr << "AFTER CALLING C1 FUNCTION!!!"<<endl;	
				cerr << "sizes are " << input->S1_pictures->size() << "\t" << input->C1_result->size() << "\n";
				cerr << "**********************************************************************************************************"<<endl;

				cout<<"after_calling C1\n";
				//pthread_exit(0);

				Picture distance;
				float min;
				for(list<list<list<Picture> > > ::iterator it1 = input->patches->begin();it1 != input->patches->end(); it1++){

					for(list<list<Picture> >::iterator it2 = (*it1).begin(); it2 != (*it1).end(); it2++){

						min = 1000000000;
						cerr<<"threadID\t"<<threadID<<"\tC1_result size is "<<input->C1_result->size()<<endl;
						for(list<list<Picture> >::iterator PicIter = input->C1_result->begin(); PicIter != input->C1_result->end(); PicIter++ ){

							window_patch_distance(*PicIter,*it2, distance);

							//cout << "printing distance in C2_function "<< endl << "*******************************" << endl;
							//print_picture(distance);
							//cout << "end of printing distance in C2_function "<< endl << "*******************************" << endl;

							float tmp = Min(distance);
							free_picture ( distance );
							min = tmp < min ? tmp : min;

						}
						//InsertInVector(input->FinalResult,min,dim1,dim2,dim3);
					        input->FinalResult->insert(input->FinalResult->begin(), dim1, dim2, dim3) = min;
						cout <<"threadID " << input->threadID  <<" MIN in C2.cpp " << min << "\t"<<dim1 <<"\t"<<dim2<<"\t"<<dim3<<endl;

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
	cerr<<"End of threadID is "<<input->threadID<<endl;
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
	int num_of_patches = 250 * 4;//TODO it should be counted following a formula

	allocate_c2_result (  pictures , C2_result ,  num_of_patches );



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


		//	cout << "c10L " << input->c10L << endl;
		input[g].threadID = g;
//		cout << "111" << endl;
		input[g].pictures = &pictures;

//		cout << "222" << endl;
		input[g].S1_pictures = S1_pictures+g;

//		cout << "333" << endl;
		input[g].C1_result = C1_result+g;

//		cout << "44444"<< endl;
		//	input[g].S2_pictures = S2_pictures+g;

//		cout << "555" << endl;
		//input[g].C2_result = C2_result_strc + g;

//		cout << "666" << endl;
		input[g].patches = &patches;

//		cout << "777" << endl;
		input[g].output = 0;
		input[g].FinalResult = &C2_result;


//		cout << "888" << endl;
		pthread_create(&thread[g], NULL, C2_function, (void*) (input+g));
		
/*
		cout << "printing C2_result at the end of run_C2_for_all_pictures  " << endl;
		for(vector<vector<vector<float> > >::iterator iter1 = C2_result.begin(); iter1 != C2_result.end(); iter1++){
			for(vector<vector<float> >::iterator iter2 = iter1->begin(); iter2 != iter1->end(); iter2++){
				for(vector<float>::iterator iter3 = iter2->begin(); iter3 != iter2->end(); iter3++)
					cout << *iter3 << "\t" ;
			}
			cout << endl;
		}*/

	}
	for(int g=0; g < NumOfThreads; g++){
		pthread_join(thread[g],NULL);
		cout << "end of thread[" << g << "] " << endl;
		if(input[g].output == -1)
			return -1;

	}






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

	rotations.push_back(0);
	rotations.push_back(45);
	rotations.push_back(90);
	rotations.push_back(-45);

	int minFS=7;
	int maxFS=37;
	int FS_step=2;
	for ( int i=minFS ; i<= maxFS ; i+=FS_step ){
		gabor_filter_sizes.push_back (i);
	}

	init_gabor_filters ( gabor_filters , gabor_filter_sizes , rotations , Div );

}


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
