#ifndef __LIBSVM__
#define __LIBSVM__
#include "../libsvm/svm.h"
#include <vector>
#include <string>

using namespace std;

int 	convert_c2_results_float_train_to_libsvm_problem ( struct svm_probmel *prob ,float ** C2_results , vector<int> sizes,int numoffeatures );
int 	convert_c2_results_float_test_to_libsvm_problem ( struct svm_probmel *prob ,float ** C2_results , vector<int> sizes,int numoffeatures );
int 	set_libsvm_parameter ( struct svm_parameter *param);
int 	svm_libsvm ( float **C2_results , vector<int> sizes , int num_of_patches , float *final_results );
void  	print_svm_problem ( svm_problem *prob , int num_of_features );
void 	print_svm_parameters ( svm_parameter *param , int num_of_patches );


#endif
