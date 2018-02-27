#include "../headers/pthread_headers.h"
#include "../headers/headers.h"
#include "../headers/gabor.h"
#include "../headers/C1.h"

void* pthread_extract_patches(void * data){

//	cout<<"pthread_extract_patches:by fahime"<<endl;

	thread_args *input_data = (thread_args*) data;
	list<Picture> trainpos = input_data -> trainpos;
	list<list<list<Picture> > > c1patches = input_data -> c1patches;
	int ntrainpos = input_data -> ntrainpos;
	list<int > patch_sizes = input_data -> patch_sizes;
	int number_of_patches = input_data -> number_of_patches;


//int extract_patches(list<Picture > &trainpos, list<list<list<Picture> > > &c1patches , int ntrainpos,list<int >patch_sizes , int number_of_patches){
  //input :

	//cerr<<"calling extract_patches\n";
	list<list<Picture> > gabor_filters;
	list<int > gabor_filter_sizes,gabor_filter_rotations;
	list<float> div;
	list<list<Picture> > S1_Pictures,C1_Pictures;
	list<Picture > patches;
	list<list<Picture> >::iterator iter_c1;
	list<Picture>::iterator iter_train_pos_list;
	vector<int > s1space(1);
	vector<int> c1scaleSS(2);
	Picture temp_random_picture;
	Picture temp_patches;
	
	//output : 
	list<list<Picture> > lltemp;
	
	div.clear();	
	c1patches.clear();
	lltemp.clear();
	S1_Pictures.clear();
	C1_Pictures.clear();
	patches.clear();
	int i,j,count=0;
	s1space[0]=10;
	c1scaleSS[0]=11;
	c1scaleSS[1]=13;
	int rotations = 4;

	//init_gaborfilter() :
	gabor_filter_sizes.push_back(11);
	gabor_filter_sizes.push_back(13);
	gabor_filter_rotations.push_back(0);
	gabor_filter_rotations.push_back(45);
	gabor_filter_rotations.push_back(90);
	gabor_filter_rotations.push_back(-45);
	div.push_back(3.9);
	div.push_back(3.85);

	//cerr<<"calling init_gabor_filters in extract_patches\n";

	if(init_gabor_filters(gabor_filters,gabor_filter_sizes,gabor_filter_rotations,div)!=0)
	{
		cout<<"error init gabor filter is not runing properly "<<endl;
		//return -1;
	}

	//cerr<<"end of calling init_gabor_filters in extract_patches\n";

	//choosing a random picture from train posetive pictures :
	for(i=0;i<number_of_patches;i++)
	{
		int r,x;
		//srand(time(NULL));
		r=rand()%ntrainpos;
		//extract a random posetive train picture form list;


		int count = 0;
		for(iter_train_pos_list=trainpos.begin();iter_train_pos_list!=trainpos.end();iter_train_pos_list++)
		{
			if(count==r)
			{
				temp_random_picture=(*iter_train_pos_list);
				count=0;
				break;
			}  
			count++;
		}

		//cerr<<"getting random picture\n";
		//print_picture ( temp_random_picture );
		//cerr<<"calling C1 for random picture\n";

		if(C1_Function(temp_random_picture,S1_Pictures,C1_Pictures,gabor_filters,s1space,c1scaleSS,2)!=0)
		{

			cerr<<"error extract_patches : C1_Function "<<endl;
			//return -1;
		}
		//cerr<<"end of calling C1 for random picture\n";

		/*list<list<Picture> > temp2;
		  list<Picture> temp1;
		  for(iter_c1=C1_Pictures.begin();iter_c1!=C1_Pictures.end();iter_c1++)
		  {
		//extract random patches call function
		temp2.pushback(temp1);
		}*/
		list<list<Picture> >::iterator iter_c1_patches;
		iter_c1_patches=C1_Pictures.begin();
		list<int >::iterator iter_patchsizes;
		for(iter_patchsizes=patch_sizes.begin();iter_patchsizes!=patch_sizes.end();iter_patchsizes++)
		{

			//cerr<<"calling init_picture to extract a patch for C1\n";
			for ( int i=0 ; i<rotations ; i++ ){

				init_picture(temp_patches,(*iter_patchsizes),(*iter_patchsizes));
				patches.push_back(temp_patches);
			}

			extract_arandom_patch((*iter_c1_patches),patches);
			lltemp.push_back(patches);
			patches.clear();
		}
		c1patches.push_back(lltemp);
		lltemp.clear();

		//end of the main for iteration :
		free_list_list_pictures ( C1_Pictures );
		free_list_list_pictures ( S1_Pictures );
	}

	input_data -> c1patches = c1patches;

	//cerr<<"extract_patches : end\n";
	
	/*for(list<list<list<Picture> > >::iterator iter = c1patches.begin();iter != c1patches.end();iter++)
		for(list<list<Picture> >::iterator iter2 = iter->begin();iter2 != iter->end();iter2 ++)
			for(list<Picture>::iterator iter3 = iter2->begin();iter3 != iter2->end();iter3++){
				print_picture(*iter3);
				cout << endl;
				cout<<"salaammmmm"<<endl;
			}*/

	return 0;
}



