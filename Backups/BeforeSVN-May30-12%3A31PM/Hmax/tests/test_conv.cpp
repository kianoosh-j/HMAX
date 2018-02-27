#include "../headers/myconv.h"
#include "../headers/picture_handler.h"
#include <iostream>


using namespace std;


int main (){

	// create 2 matrix
	Picture p1;
	Picture p2;
	Picture p3;
	init_picture ( p1 , 3 , 3 , 1 );
	init_picture ( p2 , 10 , 10 , 2 );
	init_picture ( p3 , 10 , 10 , 3 );

	imfilter_pictures_same ( p2 , p1 , p3 );

	print_picture ( p1  );
	print_picture ( p2  );
	print_picture ( p3  );

	return 0;
}
