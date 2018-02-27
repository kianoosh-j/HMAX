#ifndef __HEADERS__
#define __HEADERS__


#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <math.h>
#include <sstream>

using namespace std;

struct Picture {
	int width;
	int height;
	float *matrix;
	//newly added for statistics
	int X;
	int Y;
	int NumberInDirectory;
};

class TempType{
	public:
		string Name;
		int Number;
		TempType(string str,int num){
		Name = str;
		Number = num;
	}
};

typedef unsigned char uchar;

#endif
