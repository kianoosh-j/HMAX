############################################################################################################################################
#	run the program as below
#	1- hmax pthread
#	./hmax_pthread	1	base_folder  	destination_folder	number_of_patches
#
#	parameters means:
#
#			1:			for debug mode
#			base_folder:		folder contining the 4 folders of pictures meaning train_pos , train_neg , test_pos , test_neg	
#			destination_folder:	folder that all results will be put in
#			number_of_patches	number of patches, it will multiply by 4 as we have 4 sizes (4, 8, 12, 16)
#
#	2- hmax 101 objects
#
############################################################################################################################################

CPP_FILES = implementations/Picture.cpp \
	     implementations/Picture_Category.cpp \
	     implementations/file_processing.cpp \
	     implementations/errors.cpp\
	     implementations/C1.cpp \
	     implementations/C2.cpp \
	     implementations/picture_handler.cpp \
	     implementations/myconv.cpp \
	     implementations/gabor.cpp \
#	     implementations/pthread_patches.cpp \
#	     implementations/svm.cpp \
#	     implementations/utility_functions.cpp \


MAIN_FILE = a101_object_main.cpp

MAIN_TARGET = hmax_101_objects
ARCHIVE_TARGET = hmax_lib.a
CPP_FLAGS = `pkg-config --cflags opencv` -Iheaders/ -I../SVM/
LIBS = -lpthread `pkg-config --libs opencv`


include ../Makefile-Header
