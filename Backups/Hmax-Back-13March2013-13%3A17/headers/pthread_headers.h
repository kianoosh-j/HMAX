#include "../headers/headers.h"

#ifndef __MY_TYPE__
#define __MY_TYPE__


void* pthread_extract_patches(void * data);

struct thread_args{

	list<Picture > trainpos;
       	list<list<list<Picture> > > c1patches ;
       	int ntrainpos;
	list<int >patch_sizes;
       	int number_of_patches;

};


#endif
