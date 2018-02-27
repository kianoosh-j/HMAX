#include "headers.h"
#include "file_processing.h"
#include "C1.h"
#include "libsvm.h"
#include <ga/GASimpleGA.h>
#include <ga/GA2DBinStrGenome.h>
#include <ga/GA1DBinStrGenome.h>

#define ARGN 5
#define DIR_NUMS 4

float** C2_results_fl;
int sizes[6];
int SIZE;
float  *final_results,*final_results2;
int total_pictures, test_pos_size, test_neg_size,test2_pos_size,test2_neg_size;
float best_success_rate = 0;
int num_of_patches;
int allocate_c2_result ( list<list<Picture> >&pictures , vector<vector<vector<float> > > &C2_result , int num_of_patches);

void genetic_evolution();
float Objective(GAGenome& g);


int main( int argn , char *argv[]){

	int num_of_patches_for_each_image = 250;


	int base_index = 1;
	string base_folder ( argv[base_index++] );
	string base_c2s (argv[base_index++]); //	string base_c2_namw ( argv[base_index++]);

	//	string base_c2s (argv[1]);
	//	string base_folder;
	//	string C2_base_Name;
	//	for(int i=1;i<argn+1;i++){
	//		if(i==1){
	//		base_folder=argv[i];
	//		}
	//		if(i==2){
	//		C2_base_Name=argv[i];
	//		}
	//	}
	//	string path=base_folder+C2_base_Name;
	//	string train_pos=path+"train_pos/",train_neg=path+"train_neg/",test_pos=path+"test_pos/",test_neg=path+"test_neg/",test2_pos=path+"test2_pos/"
	//		,test2_neg=path+"test2_neg/";
	//	
	//	fstream file_train_pos,file_train_neg,file_test_pos,file_test_neg,file_test2_pos,file_test2_neg;
	//	file_train_pos.open(train_pos.c_str(),ios_base::in);
	//	file_train_neg.open(train_neg.c_str(),ios_base::in);
	//	file_test_pos.open(test_pos.c_str(),ios_base::in);
	//	file_test_neg.open(test_neg.c_str(),ios_base::in);
	//	file_test2_pos.open(test2_pos.c_str(),ios_base::in);
	//	file_test2_neg.open(test2_neg.c_str(),ios_base::in);
	//
	//	for(
	//




	//	file_train_neg.open();



	list<string> c2_files;
	cerr<<"test1 :in  : befor c2_file init  "<<endl;

	c2_files.push_back ( base_folder+base_c2s + "_train_pos.txt" );
	c2_files.push_back ( base_folder+base_c2s + "_train_neg.txt" );
	c2_files.push_back ( base_folder+base_c2s + "_test_pos.txt"  );
	c2_files.push_back ( base_folder+base_c2s + "_test_neg.txt"  );
	c2_files.push_back ( base_folder+base_c2s + "_test2_pos.txt" );
	c2_files.push_back ( base_folder+base_c2s + "_test2_neg.txt" );
	cerr<<"test 2 : after c2_files init "<<endl;


	total_pictures = 0;
	fstream file_size;
	string path_to_file_size=base_folder+"size";
	file_size.open(path_to_file_size.c_str(),ios_base::in);

	if(file_size.fail()==1)
	{
		cerr<<"error : reading from file size is not working properly "<<endl;
		return 0;
	}

	cerr<<"test3 : before reading form file_size "<<endl;
	for(int i=0;i<c2_files.size();i++){
		file_size>>sizes[i];
		total_pictures+=sizes[i];
	}
	file_size.close();

	cerr<<"test4 : after reading file_size "<<endl;

	/*	for ( int i=0 ; i<c2_files.size();i++ ){
		sizes[i] = atoi ( argv[base_index++]) ;
		total_pictures += sizes[i];
		}*/

	num_of_patches = atoi ( argv[base_index++]);

	cerr<<"test5 : before allocating c2_result_fl and total_pictures="<<total_pictures<<endl;

	C2_results_fl = new float * [ total_pictures];
	for ( int i=0 ; i<total_pictures ; i++ ){
		C2_results_fl[i] = new float [ num_of_patches];
	}

	cerr<<"test6 : befor test2_pos "<<endl;

	test_pos_size = sizes[2];
	test_neg_size = sizes[3];
	test2_pos_size= sizes[4];
	test2_neg_size= sizes[5];


	final_results = new float [test_pos_size + test_neg_size ];
	final_results2= new float [test2_pos_size+test2_neg_size ];



	int count = 0;
	fstream fin;
	int index=0;
	for(list<string>::iterator iter = c2_files.begin(); iter != c2_files.end(); iter++ ){

		fin.open(iter->c_str() , ios::in);
		if(fin.fail()==1)
		{
			cerr<<"error : "<<iter->c_str()<<" in reading C2_results files an error has occured !!  "<<endl;
			return -1;
		}
		cerr<<"test6 : count="<<count<<endl;

		for(int i = 0; i < sizes[count]; i++ )
			for(int j = 0; j < num_of_patches ; j++){
				fin >> C2_results_fl[i+index][j];
				//cout<< C2_results_fl[i+index][j]<<" ";
				j = (j + 1) % num_of_patches;
			}
		//	cout << C2_results_fl[][ ] << "\t";
		fin.close();
		//cout<<endl;
		index += sizes[count];
		count++;
	}
	cerr<<"test7 : before evolution "<<endl;

	genetic_evolution();	

	return 0;

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


//selection for test2 
float Objective2( GAGenome& g){


	GA1DBinaryStringGenome &genome = (GA1DBinaryStringGenome &)g;

	int selection_count = 0;    
	int NumOfSelectedPatches = 0;
	// use parameter instead of constant 1000
	for(int i=0; i < num_of_patches; i++)
		NumOfSelectedPatches = genome.gene(i) ? NumOfSelectedPatches+1 : NumOfSelectedPatches;

	float** result_selected = new float*[sizes[0]+sizes[1]+sizes[4]+sizes[5] ];
	for(int i=0; i < sizes[0]+sizes[1]; i++){
		result_selected[i] = new float[NumOfSelectedPatches];

		//use a parameter instead of constant
		selection_count=0;
		for(int j=0; j < num_of_patches; j++)
			if(genome.gene(j)){
				result_selected[i][selection_count] = C2_results_fl[i][j];
				selection_count++;
			}	
	}

	selection_count=0;




	for(int i=sizes[0]+sizes[1]+sizes[2]+sizes[3]-1; i < total_pictures; i++){
		result_selected[i] = new float[NumOfSelectedPatches];

		//use a parameter instead of constant
		selection_count=0;
		for(int j=0; j < num_of_patches; j++)
			if(genome.gene(j)){
				result_selected[i][selection_count] = C2_results_fl[i][j];
				selection_count++;
			}	
	}





	if ( NumOfSelectedPatches == 0 )
		return 0;

	vector<int> vsizes (4);
	for ( int i=0 ; i< vsizes.size() ; i++ ){
		vsizes[i] = sizes[i];
	}

	svm_libsvm ( result_selected , vsizes , NumOfSelectedPatches, final_results );

	int success_rate=0;

	//cout<<"\n\n**********************************************************\n\n                     Pos results\n\n**********************************************************\n\n"; 
	//cout<<"kianoosh main test test_pos_size "<<test_pos_size <<endl;
	for ( int i=0 ; i<test2_pos_size ; i++){
		//cout<<final_results[i]<<endl;
		if ( final_results[i]==1 )
			success_rate++;
	}

	//cout<<"\n\n**********************************************************\n\n                     Neg results\n\n**********************************************************\n\n"; 
	for ( int i=0 ; i<test2_neg_size ; i++){
		//cout<<final_results[i+test_pos_size]<<endl;
		if ( final_results[i+test_pos_size]==-1 )
			success_rate++;
	}
	//delete result_selected
	for( int i=0; i <sizes[0]+sizes[1]+sizes[4]+sizes[5];i++ )
		delete[]result_selected[i];
	delete []result_selected;

	cerr <<"**************************** Reached the end of one generation *******************************" << endl;
	float fitness = (float(success_rate)/(test_pos_size+test_neg_size));
	if ( fitness > best_success_rate ){
		cerr<<"the genome is\n"<<g<<endl;
		best_success_rate = fitness;
		cerr<<"best success rate "<<fitness<<endl;

	}
	cout << "Final success rate  " << best_success_rate << endl;
	return fitness;

	//return (float)(rand()%1000)/1001;

}


float Objective( GAGenome& g){


	GA1DBinaryStringGenome &genome = (GA1DBinaryStringGenome &)g;

	int selection_count = 0;    
	int NumOfSelectedPatches = 0;
	// use parameter instead of constant 1000
	for(int i=0; i < num_of_patches; i++)
		NumOfSelectedPatches = genome.gene(i) ? NumOfSelectedPatches+1 : NumOfSelectedPatches;

	float** result_selected = new float*[sizes[0]+sizes[1]+sizes[2]+sizes[3]];
	for(int i=0; i < sizes[0]+sizes[1]+sizes[2]+sizes[3]; i++){
		result_selected[i] = new float[NumOfSelectedPatches];

		//use a parameter instead of constant
		selection_count=0;
		for(int j=0; j < num_of_patches; j++)
			if(genome.gene(j)){
				result_selected[i][selection_count] = C2_results_fl[i][j];
				selection_count++;
			}	
	}

	if ( NumOfSelectedPatches == 0 )
		return 0;
	cerr<<"test10 : before svm "<<endl;

	vector<int> vsizes (4);
	for ( int i=0 ; i< vsizes.size() ; i++ ){
		vsizes[i] = sizes[i];
	}

	svm_libsvm ( result_selected , vsizes , NumOfSelectedPatches, final_results );
	//svm ( result_selected , sizes[0],sizes[1],sizes[2],sizes[3], NumOfSelectedPatches, final_results );
	cerr<<"test11 : after svm "<<endl;

	int success_rate=0;

	//cout<<"\n\n**********************************************************\n\n                     Pos results\n\n**********************************************************\n\n"; 
	//cout<<"kianoosh main test test_pos_size "<<test_pos_size <<endl;
	for ( int i=0 ; i<test_pos_size ; i++){
		//cout<<final_results[i]<<endl;
		if ( final_results[i]==1 )
			success_rate++;
	}

	//cout<<"\n\n**********************************************************\n\n                     Neg results\n\n**********************************************************\n\n"; 
	for ( int i=0 ; i<test_neg_size ; i++){
		//cout<<final_results[i+test_pos_size]<<endl;
		if ( final_results[i+test_pos_size]==-1 )
			success_rate++;
	}
	//delete result_selected
	for( int i=0; i <sizes[0]+sizes[1]+sizes[2]+sizes[3] ; i++ )
		delete[]result_selected[i];
	delete []result_selected;

	cerr <<"**************************** Reached the end of one generation *******************************" << endl;
	float fitness = (float(success_rate)/(test_pos_size+test_neg_size));
	if ( fitness > best_success_rate ){
		cerr<<"the genome is\n"<<g<<endl;
		best_success_rate = fitness;
		cerr<<"best success rate "<<fitness<<endl;

	}
	cout << "Final success rate  " << best_success_rate << endl;
	return fitness;

	//return (float)(rand()%1000)/1001;
	cerr<<"ennd of the objective "<<endl;

}


void genetic_evolution(){

	fstream gfile;
	gfile.open("sgenome",ios_base::out);

	//	unsigned int seed = 10;
	GARandomSeed(rand());	

	//!!! use parameter
	int width = num_of_patches;
	int height = 1;
	int popsize = 70;
	int ngen = 150;
	float pmut = 0.01;
	float pcross = 0.6;

	GA1DBinaryStringGenome genome(width, Objective);

	GASimpleGA ga(genome);
	ga.populationSize(popsize);
	ga.nGenerations(ngen);
	ga.pMutation(pmut);
	ga.pCrossover(pcross);
	ga.evolve();
	//*****
	cout << "******************" << endl;
	cout <<"## BEST INDIVIDUAL ## "<< ga.statistics() << endl;
	cout << "******************" << endl;
	cerr <<"## BEST INDIVIDUAL ## "<< ga.statistics().bestIndividual() << endl;
	gfile<<ga.statistics().bestIndividual();
	cout << "******************" << endl;
	//Objective2( ga.genome );

}
