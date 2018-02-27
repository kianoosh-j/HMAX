#include "highgui.h"
#include "ml.h"
#include "../headers/headers.h"

using namespace std;

int svm(float** input, int size1,int size2,int size3,int size4,int num_feature, float* result){
	cout << "first line" << endl;
	float labels[size1+size2] ;
	for(int i=0; i<size1; i++){
		labels[i]=1;
	}
	for(int j=size1; j<size1+size2; j++){
		labels[j]=-1;
	}
	cout << "MIDDLE" << endl;
	cv::Mat labelsMat(size1+size2, 1, CV_32FC1, labels);
	cout << "Aftter CV::MAT :P" << endl;
	float trainingData[(size1+size2)*(num_feature)];
	for(int j=0; j<size1+size2; j++){
		for(int i=0; i<num_feature; i++){
			trainingData[j*(num_feature)+i]=input[j][i];
		}

	}
	cout << "BOOOOOOOO" << endl;
	cv::Mat trainingDataMat(size1+size2, num_feature, CV_32FC1, trainingData);
	CvSVMParams params;
	params.svm_type	= CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	CvSVM SVM;
	SVM.train(trainingDataMat, labelsMat,cv::Mat(),cv::Mat(), params);
	cout << "yeeeeeeeeeeeeees" << endl;
	for(int j=size1 + size2; j<size3+size4+size1+size2; j++){
		int index = 0;
			cv::Mat sampleMat(1, num_feature,  CV_32FC1, input[j] );
			result[j-size1-size2] = SVM.predict(sampleMat);

	}

	return 0;

}	



