#ifndef __C1__
#define __C1__

#include "headers.h"
#include "picture_handler.h"

void 	init_parameters_for_C1 ( list<list<Picture> > &gabor_filters , vector<int> &c1SpaceSS , vector<int> &c1ScaleSS , int &c10L );

//int 	extract_patches(list<Picture > &trainpos, list<list<list<Picture> > > &c1patches , int ntrainpos,list<int >patch_sizes , int number_of_patches);
int 	extract_patches(list<Picture > &trainpos, list<list< Patch > > &c1patches , int ntrainpos,list<int >patch_sizes , int number_of_patches);

//int 	extract_arandom_patch(list<Picture> &picture, list<Picture> &patch);
int 	extract_arandom_patch(list<Picture>& picture, Patch& patch);

int 	extract_patches_from_S1 ( const list< list<Picture > > &gabored_pictures , list< list<Picture> > &patches , const list<int> &patch_sizes , const int num_of_patches_for_each_image );

int 	normalize_picture ( const Picture &picture , int radius , Picture &normalized_picture );

int 	run_gabor_filter_on_pictures ( list< Picture> &pictures , list< list<Picture> > &gabor_filters , list< list<list<Picture> > > &gabored_pictures );

int 	remove_borders_list ( list<Picture> &gabored_pictures , const list<int> gabor_filter_sizes , int num_of_rotations );

int 	remove_borders ( const Picture &picture , int left, int up ,int right ,int bot );

int 	max_filter ( list<Picture> &gabored_pictures , int local_width , int local_height );

int 	free_all_memories ( list< list<float *> > &gabor_filters , list< list<Picture> > &gabored_pictures , list <list <Picture> > &normalized_pictures );

int 	max_list_pictures ( list<Picture> &first_pics , list<Picture> &second_pics );

int 	init_list_picture ( list<Picture> &dest , list<Picture>&src );

int 	C1_Function ( Picture &picture, list<list<Picture> > &S1_Pictures, list<list<Picture> > &C1_Pictures , list<list<Picture> > &gabor_filters, const vector<int> &c1SpaceSS, const vector<int> &c1ScaleSS, int c10L );

int 	C1_Function_List ( list<Picture> &picture, list<list<list<Picture> > > &S1_Results, list<list<list<Picture> > > &C1_Results , list<list<Picture> > &gabor_filters, const vector<int> &c1SpaceSS, const vector<int> &c1ScaleSS, int c10L );

int 	local_max ( Picture &picture , int local_width , int local_height );

int 	sub_sample ( const Picture &picture , Picture &sub_picture , int width_scale , int height_scale );

void 	SumFilter ( float *matrix , int width , int height , int radius);// do the some filter notice that it should get width and height

int 	SumFilter ( Picture pic , int radius[] , int size );

void 	Radical2 ( float *matrix , int size );// it should allocate new array for dest and puts radical source[i] in dest[i]

void 	Plus_Not ( float *matrix , int size );// it should put matrix[i] = matrix[i] + ~matrix[i]  if matrix[i] is non zero ~matrix[i] will be zero otherwise it is one

void 	Power2 ( float *source , float * dest , int size );// it should allocate new array for dest and puts source[i]*source[i] in dest[i]


#endif
