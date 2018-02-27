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


############################################  TARGETS #####################################################################################
TARGET = hmax_pthread
TEST_OPENCV = tests/test_conv
###########################################################################################################################################


########################################### FILES AND LIBRAYRIES AND COMPILATION HINTS ####################################################
BASE_FILES = implementations/C1.cpp \
	     implementations/C2.cpp \
	     implementations/gabor.cpp \
	     implementations/myconv.cpp \
	     implementations/picture_handler.cpp \
	     implementations/pthread_patches.cpp \
	     implementations/svm.cpp \
	     implementations/file_processing.cpp \
	     implementations/utility_functions.cpp

HMAX_MAIN_FILE = hmax_main.cpp

TEST_CONV_MAIN = tests/test_conv.cpp
	     
LIBS = ../SVM/libsvm.a


INCLUDE_PATH = ../SVM/


FLAGS = `pkg-config --cflags opencv`


LINKS = -lpthread `pkg-config --libs opencv`


BASE_OBJ = ${BASE_FILES:.cpp=.o}
HMAX_MAIN_OBJ = ${HMAX_MAIN_FILE:.cpp=.o}
TEST_CONV_MAIN_OBJ = ${TEST_CONV_MAIN:.cpp=.o}
###########################################################################################################################################


all: ${TARGET} ${TEST_OPENCV}

######################################################################## TARGET COMPILATIONS #############################################
${TARGET}: ${BASE_OBJ} ${HMAX_MAIN_OBJ}
	${compiler}  	-I${INCLUDE_PATH} 	${FLAGS} 	-o 	${TARGET} 	${BASE_OBJ} ${HMAX_MAIN_OBJ} 	${LIBS} 	${LINKS}
	
clean:
	rm ${BASE_OBJ} *~ ${TARGET}

.cpp.o:
	${compiler} ${FLAGS} -c $< -o $@


${TEST_OPENCV} : ${BASE_OBJ} ${TEST_CONV_MAIN_OBJ}
	${compiler}  	-I${INCLUDE_PATH} 	${FLAGS} 	-o 	${TEST_OPENCV} 	${BASE_OBJ} ${TEST_CONV_MAIN_OBJ} 	${LIBS} 	${LINKS}
###########################################################################################################################################



