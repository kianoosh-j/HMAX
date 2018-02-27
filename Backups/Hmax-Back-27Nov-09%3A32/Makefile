############################################################################################################################################
#	run the program as below
#	./hmax_pthread	1	base_folder  	destination_folder
#
#	parameters means:
#
#			1:			for debug mode
#			base_folder:		folder contining the 4 folders of pictures meaning train_pos , train_neg , test_pos , test_neg	
#			destination_folder:	folder that all results will be put in
#
############################################################################################################################################
compiler=g++
implementation_dir=implementations
headers_dir=headers


TARGET = hmax_pthread


BASE_FILES = implementations/C1.cpp \
	     implementations/C2.cpp \
	     implementations/gabor.cpp \
	     implementations/myconv.cpp \
	     implementations/picture_handler.cpp \
	     implementations/pthread_patches.cpp \
	     implementations/svm.cpp \
	     implementations/file_processing.cpp \
	     implementations/utility_functions.cpp \
	     hmax_main.cpp

	     
LIBS = libs/libsvm.o \
       libs/svm.o


FLAGS = `pkg-config --cflags opencv`


LINKS = -lpthread `pkg-config --libs opencv`


BASE_OBJ = ${BASE_FILES:.cpp=.o}


all: ${TARGET}


${TARGET}: ${BASE_OBJ}
	${compiler}  ${FLAGS} -o ${TARGET} ${BASE_OBJ} ${LIBS} ${LINKS}
	
clean:
	rm ${BASE_OBJ} *~

.cpp.o:
	${compiler} ${FLAGS} -c $< -o $@

