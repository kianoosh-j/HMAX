#include "../headers/libsvm.h"
#include <iostream>

int convert_c2_results_float_train_to_libsvm_problem ( struct svm_problem *prob ,float ** C2_results , vector<int> sizes,int numoffeatures )
{	
	int train_size=sizes[0]+sizes[1];
	prob->l=0;
	prob->l=train_size;
	prob->y =new double [train_size]; 
	prob->x =new svm_node *[train_size];
	svm_node *x_space = new svm_node [train_size * (numoffeatures+1) ];

	for(int i=0;i<train_size;i++)
		prob->x[i]=x_space + i*(numoffeatures+1);

	for(int i=0;i<sizes[0];i++)
		prob->y[i]=1;

	for(int i=sizes[0];i<train_size;i++)
		prob->y[i]=-1;

	for(int i=0;i<train_size;i++){
		for(int j=0;j<numoffeatures;j++){
			prob->x[i][j].value=C2_results[i][j];
			prob->x[i][j].index=j;
		}
		prob->x[i][numoffeatures].index =-1;
	}
			
	return 0;

}
int convert_c2_results_float_test_to_libsvm_problem ( struct svm_problem *prob ,float ** C2_results , vector<int> sizes,int numoffeatures )
{
	int test_size=sizes[2]+sizes[3];
	int base_size = sizes[0] + sizes[1];

	prob->l=test_size;
	prob->y =new double [test_size];
	prob->x = new svm_node *[test_size];
	svm_node *x_space =new svm_node  [test_size * (numoffeatures+1)];

	for(int i=0;i<test_size;i++)
		prob->x[i]= x_space + i*(numoffeatures+1);

	for(int i=0;i<sizes[2];i++)
		prob->y[i]=1;

	for(int i=sizes[2];i<test_size;i++)
		prob->y[i]=-1;

	for(int i=0;i<test_size;i++){
		for(int j=0;j<numoffeatures;j++){
			prob->x[i][j].value=C2_results[i+base_size][j];
			prob->x[i][j].index=j;
		}
		prob->x[i][numoffeatures].index = -1;
	}

	return 0;

}


int set_libsvm_parameter ( struct svm_parameter *param)
{

	// default values
	param->svm_type = C_SVC;
	param->kernel_type = RBF;
	param->degree = 3;
	param->gamma = 0;	// 1/num_features
	param->coef0 = 0;
	param->nu = 0.5;
	param->cache_size = 100;
	param->C = 1;
	param->eps = 1e-3;
	param->p = 0.1;
	param->shrinking = 1;
	param->probability = 0;
	param->nr_weight = 0;
	param->weight_label = NULL;
	param->weight = NULL;
	//cross_validation = 0;

}


int svm_libsvm ( float **C2_results , vector<int> sizes , int num_of_patches , float *final_results ){


	svm_problem prob_train , prob_test;
	svm_parameter param;
	svm_model *model;
	const char *file_name="libsvm";

	convert_c2_results_float_train_to_libsvm_problem ( &prob_train , C2_results , sizes , num_of_patches );
	set_libsvm_parameter ( &param );
	param.gamma = 1.0 / num_of_patches;


 	const char *err_msg=svm_check_parameter ( &prob_train , &param );
	if(err_msg){
		cerr<<"error :error in parameters : "<<endl;
	}
	//cout<<"printing parameters before calling svm_train function : \n";
	//print_svm_parameters (&param,num_of_patches);
	//cout<<"\n\nprinting problem : "<<endl;
	//print_svm_problem(&prob_train,num_of_patches);
	
	cerr<<"\n\nbefore creating model "<<endl;
	model = svm_train ( &prob_train , &param );
	cerr<<"after creating model "<<endl;
	//svm_print_model(model,num_of_patches);

	//cout<<endl<<endl;
	//cout<<"moedel->sv : "<<endl;
	//svm_print_model(model,num_of_patches);


	//svm_save_model(file_name,model);

	cerr<<"before creating c2 for libsvm\n";
	convert_c2_results_float_test_to_libsvm_problem ( &prob_test , C2_results , sizes , num_of_patches );
	cerr<<"after creating c2 for libsvm\n";

	for ( int i = 0 ; i<sizes[2] + sizes[3] ; i++){
		 final_results[i] = svm_predict ( model , prob_test.x[i] );
	}

	svm_free_and_destroy_model ( &model );

}



void  print_svm_problem ( svm_problem *prob , int num_of_features ){
	
	cout<<"problem spec is \n";
	cout<<prob->l<<endl;
	for ( int i=0 ; i<prob->l ; i++ ){

		cout<<prob->y[i]<<" ";
		for ( int j=0 ; j<num_of_features ; j++ ){

			cout<<prob->x[i][j].index<<":"<<prob->x[i][j].value<<" ";
		}
		cout<<endl;
	}
	cout<<endl;

	for(int i=0;i<prob->l;i++)
		cout<<prob->y[i]<<endl;

			
}


//void print_svm_parameters ( svm_parameter *param , int num_of_patches ){

//	cout<<" svm_type : "<<param->svm_type<<endl;
//	cout<<" kernel_type : "<<param->kernel_type<<endl;
//	cout<<" degree : "<<param->degree<<endl;	/* for poly */
//	cout<<" gamma : "<<param->gamma<<endl;	/* for poly/rbf/sigmoid */
//	cout<<" coef0 : "<<param->coef0<<endl;	/* for poly/sigmoid */

	/* these are for training only */
//	cout<<" cache_size : "<<param->cache_size<<endl; /* in MB */
//	cout<<" eps : "<<param->eps<<endl;	/* stopping criteria */
//	cout<<" C; : "<<param->C<<endl;	/* for C_SVC, EPSILON_SVR and NU_SVR */
//	cout<<" nr_weight : "<<param->nr_weight<<endl;		/* for C_SVC */
//	cout<<"*************"<<endl;

  //      cout<<" nu : "<<param->nu<<endl;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
//	cout<<" p; : "<<param->p<<endl;	/* for EPSILON_SVR */
//	cout<<" shrinking : "<<param->shrinking<<endl;	/* use the shrinking heuristics */
//	cout<<" probability : "<<param->probability<<endl; /* do probability estimates */
///	cout<<"*********"<<endl;


//	cout<<" weight_label : "<<param->weight_label<<endl;   //this part is my test 
///	cout<<" *weight_label : "<<*(param->weight_label)<<endl;	/* for C_SVC */
//	cout<<"* weight : "<<*(param->weight)<<endl;		/* for C_SVC */
//	}
