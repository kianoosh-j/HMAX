#ifndef __PICTURE_HANDLER__
#define __PICTURE_HANDLER__

#include "headers.h"
#include "Picture.h"
#include "Patch.h"

int 	max_pictures ( Picture result , Picture first , Picture second );
void 	free_picture ( Picture &p);
int 	divide_pictures ( Picture &dest , const Picture &division, const Picture &divisor);
int 	init_copy_picture ( Picture &dest , Picture &src );
bool 	equal_size_pictures ( const Picture &p1 , const Picture &p2 );
bool 	valid_picture ( const Picture & p );
int 	init_picture ( Picture &p , int w , int h );
int 	init_picture ( Picture &p , int w , int h , int value);
void 	print_picture (Picture p);
bool 	init_picture_matrix ( Picture &p);
int 	abs_matrix ( float *dest , float *src , int size );
int 	inverse_picture ( Picture &pic );
void 	print_matrix ( float *a , int w , int h );
void 	read_matrix ( float *a , int w , int h );
void 	free_list_list_pictures ( list<list<Picture> > &pictures );
void 	free_list_pictures ( list<Picture> pictures );
bool 	equal_list_pictures ( list<Picture> &pictures );
int 	create_random_picture ( Picture &pic , int width , int height );
int	read_picture_from_file ( Picture &pic , string file_name );
int	read_patch_from_file ( Picture &pic , string file_name );
void 	print_list_list_picture ( const list<list<Picture> > &llpictures );
void 	print_list_picture ( const list<Picture> &lpictures );

//NEWMS
// show an image using opencv Mat class
void	show_image ( Mat &img );
// show an image using Picture class
void	show_image ( Picture &picture );
// show an image usging image file name
void	show_image ( string image_file_name );

#endif
