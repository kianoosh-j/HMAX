#include<iostream>
#include"c2class.h"

using namespace std;

C2_Output::C2_Output(int init_num_of_patches,int init_num_of_pictures,int init_num_of_classes){

	num_of_patches = init_num_of_patches;
	num_of_pictures = init_num_of_pictures;
	num_of_classes = init_num_of_classes;
	
	C2Results = new float*[num_of_pictures];
	for(int i=0 ;i<num_of_pictures;i++)
	       C2Results[i] = new float[num_of_patches];

	Labels = new int*[num_of_pictures];
	for(int i=0;i<num_of_pictures;i++)
		Labels[i] = new int [3];
}

float** C2_Output::getC2Results(){
	return C2Results;
}


int** C2_Output::getLabels(){
	return Labels;
}


int C2_Output::getNumofPatches(){
	return num_of_patches;
}



int C2_Output::getNumofPictures(){
	return num_of_pictures;
}


int C2_Output::getNumofclasses(){
	return num_of_classes;
}

void C2_Output::show(){
	cout<<"num of patches : "<<num_of_patches<<endl;
	cout<<"num of pictures : "<<num_of_pictures<<endl;
	cout<<"num of classes  : "<<num_of_classes<<endl;
	for(int i=0;i<num_of_pictures;i++){
		for(int j=0;j<num_of_patches;j++)
			cout<<" "<<C2Results[i][j];
		cout<<endl;
	}
			
}

int main(){

C2_Output c2_cell(5,15,3);
float **f = c2_cell.getC2Results();
for(int i=0;i<15;i++){
	for(int j=0;j<5;j++)
		f[i][j] = i*j;
}

int **label=c2_cell.getLabels();
for(int i=0;i<15;i++){
   for(int j=0;j<3;j++)
	   label[i][j] = i;
}
cout<<"before calling show function "<<endl;

c2_cell.show();

return 0;

}

