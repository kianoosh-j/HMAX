#ifndef __SVM__
#define __SVM__

int svm(float** input,int size1,int size2,int size3,int size4,int num_fiture, float* result);
// it computes success rate from C2_results , it also needs number of patches
float compute_success_rate ( list<list<Picture> > &pictures , float ** C2_results_fl , int number_of_patches );

#endif
