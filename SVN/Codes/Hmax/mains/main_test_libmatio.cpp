#include "../headers/headers.h"
#include "../headers/PatchIO.h"
using namespace std;
int main(){

	PatchIO patchio;
	const string addr = "../PatchesFromNaturalImages.mat";
	//char* var_name = "cPatches" ;
	char* var_name = "numPatchSizes" ;
	//patchio.ReadPatch2( addr, var_name );	
	
	
	/********  Writing A Patch  ************/

	//const double a[5] = {43.5, 5432.434, 3.32, 0.0001, 88834.0};
	//int dims[2] = {1,5};
	Patch patch;
	const string dest_file_name = "simple.mat";
	patchio.WritePatch( patch , dest_file_name );	
	return 0;

}

