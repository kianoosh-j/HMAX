#include "../headers/Picture_Category.h"


Picture_Category::Picture_Category(string category_name , int category_label){
	pictures.resize(0);
	this->category_name = category_name;
	this->category_label = category_label;
}

Picture_Category::Picture_Category(){
}

void Picture_Category::set_Category_Label(int category_label){
	this->category_label = category_label;
}

Picture_Category::~Picture_Category(){
}

void Picture_Category::add_Picture(Picture &picture){
	pictures.push_back(picture);
}

vector<Picture> *Picture_Category::get_pictures_reference(){
	return &pictures;
}
