#include<iostream>

using namespace std;

class C2_Output{

	float **C2Results;
	int **Labels;
	int num_of_patches;
	int num_of_pictures;
	int num_of_classes;
	const static int default_num_of_patchest = 1000;

	public :

	C2_Output(int init_num_of_patches,int init_num_of_pictures,int init_num_of_classes);
	float** getC2Results();
	int** getLabels();
	int getNumofPatches();
	int getNumofPictures();
	int getNumofclasses();
	void show();
};


