#ifndef __PATCHIO__
#define __PATCHIO__

#include "headers.h"
#include "Picture.h"

using namespace std;

class PatchIO{

	public:

		int ReadAllPatches(){
			return 1;
		}

		/*
		   int ReadPatch( const string& file_name ){

		   mat_t *matftp = Mat_Open( file_name.c_str(), MAT_ACC_RDONLY );
		   if( matftp == NULL ){

		   cout << "Error Reading Mat file in PATCHIO" << endl;
		   return false;

		   }

//matvar_t *test = NULL;


matvar_t* matvar;
matvar = Mat_VarCreate("vec1",MAT_C_DOUBLE,MAT_T_DOUBLE,2,dims,a,0);
Mat_VarWrite( mat, matvar, 0);

Mat_VarFree(matvar);
Mat_Close(mat);
do{
test = Mat_VarReadNextInfo(matftp);
if(test==NULL) continue;
cout << test->name << endl;

}
while(test);

return true;



}
*/

/*
int ReadPatch(const string& file_name ){

	
	const double a[5] = {43.5, 5432.434, 3.32, 0.0001, 88834.0};

	mat_t *mat;
	matvar_t *matvar;
	int dims[2] = {1,5};

	mat = Mat_Open(file_name.c_str(),MAT_ACC_RDONLY);
	void* data;

	if(mat)
	{
	matvar = Mat_VarCreate( "vec1", MAT_C_DOUBLE, MAT_T_DOUBLE,2,dims,a,0 );
	Mat_VarReadInfo(mat, cPatches);
	Mat_VarReadData( mat, matvar, data );

	//Mat_VarFree(matvar);
	//Mat_Close(mat);

	return 0;
	}
	else
	return -1;
	

	int   start[2]={0,0},stride[2]={1,1},edge[2]={2,2}, err = 0;
	double ptr[4];
	mat_t  *mat;
	matvar_t *matvar;
	const char* var = "cPatches";

	fpos = ftell( mat->fp );
	fseek( mat->fp, mat->bof, SEEK_SET);
	do{

		err = fread(&data_type,4,1,mat->fp);
		if ( !err )
			return NULL;
		err = fread(&nBytes,4,1,mat->fp);
		if ( mat->byteswap ) {
			Mat_int32Swap(&data_type);
			Mat_int32Swap(&nBytes);

		}


		curpos = ftell(mat->fp);
		fseek(mat->fp,-8,SEEK_CUR);

		matvar = Mat_VarReadNextInfo(mat);
		if ( matvar != NULL ) {
			if ( !matvar->name ) {
				Mat_VarFree(matvar);
				matvar = NULL;
			} else if ( strcmp(matvar->name,name) ) {
				Mat_VarFree(matvar);
				matvar = NULL;
			}
		} else {
			Mat_Critical("An error occurred in reading the MAT file");
			break;
		}
		fseek(mat->fp,curpos+nBytes,SEEK_SET);



	}while ( !matvar && !feof(mat->fp) );

	fseek(mat->fp,fpos,SEEK_SET);
	return matvar;    
			     mat = Mat_Open(file_name.c_str() , MAT_ACC_RDONLY);
			     if ( mat ) {
			     matvar = Mat_VarReadInfo(mat, (char*) var );
			     if ( matvar != NULL ) {
			     stride[0] = matvar->dims[0]-1;
			     stride[1] = matvar->dims[1]-1;
			     Mat_VarReadData(mat,matvar,ptr,start,stride,edge);
			     printf("%f    %f\n%f    %f\n",ptr[0],ptr[1],ptr[2],ptr[3]);
			     Mat_VarFree(matvar);
			     } else {
			     err = 1;
			     }
			     Mat_Close(mat);
			     } else {
			     err = 1;
			     }
			     return err;
			     
}
*/
int ReadPatch2( const string& file_name, char* name ){




	long  fpos;
	mat_t* mat;

	mat = Mat_Open(file_name.c_str(),MAT_ACC_RDONLY);
 	matvar_t *matvar = Mat_VarReadInfo(mat, name );

	cout << "print matvar" << endl;
	//Mat_VarPrint( matvar, 1);
 	cout << Mat_VarGetNumberOfFields( matvar ) << endl;
	cout << "end printing matvar" << endl;
	if ( (mat == NULL) || ( name == NULL) ){
		return -1;
	}

	int height = matvar->dims[0]; 
	int width = matvar->dims[1];
	cout << " before initialization " << endl;	
	int stride[2] = { height - 1, width - 1 };
	int start[2] = { 0, 0 };
	int edge[2] = { height, width };
	int* data = NULL;

	cout << "after initialization " << endl;	
	fpos = ftell(mat->fp);
	matvar = Mat_VarReadInfo(mat,name);
	cout << "size of matvar:  " <<  sizeof(matvar) << endl;
	cout << "after ReadInfo" << endl;
	cout << sizeof(data) << endl;
	if ( matvar ){
		cout << "here" << endl;
		Mat_VarReadData(mat,matvar,( void* ) data, start, stride, edge);
	}
	if( data == NULL ){
		cout << "data is null!!" << endl;
	} 
	fseek(mat->fp,fpos,SEEK_SET);
	cout << "after fseek" << endl;
	
	cout << sizeof(data) << endl;
	cout << (*data) << endl;
	/*for( int i=0; i < height; i++){
		for(int j=0; j < width; j++){
			cout << data[ i*height + width ] << "\t"; 
		}
		cout << endl;
	}*/

	Mat_VarFree( matvar );
	Mat_Close( mat );
	return 0;
}

int WritePatch( Patch& patch, const string& dest_file_name ){


	int height = patch.patches_in_different_orientations[0].height;
	int width = patch.patches_in_different_orientations[0].width;
	int dims[2] = { height, width};

	mat_t *mat;
	matvar_t *matvar;

	mat = Mat_Open( dest_file_name.c_str() ,MAT_ACC_RDWR);
	string base = "vec";
	string str[4] = {"0", "1", "2", "3"}; 

	
	if(mat)
	{

		for( int i=0; i < patch.patches_in_different_orientations.size(); i++ ){
			const char*  str = ( base + str[i] ).c_str();
			double* matrix = patch.patches_in_different_orientations[i].matrix;

			matvar = Mat_VarCreate( str ,MAT_C_DOUBLE,MAT_T_DOUBLE,2,dims,(void*) matrix,0);
			Mat_VarWrite( mat, matvar, 0);
			Mat_VarFree(matvar);
		}



		Mat_Close(mat);
	}
	else
		return 1;

	return 0;

}


};

#endif
