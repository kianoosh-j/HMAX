############################################################################################################################################
#	
#  Run the Code like belew
#              ./hmax_genetic   base_folder    base_name_of_c2_results   num_of_patches
#
#              for example:
#              ./hmax_genetic   ../../../Results/res3_face_1000/    C2Results    1000
#	
#
############################################################################################################################################
COMPILER=g++


TARGET = hmax_genetic


BASE_FILES = main_genetic.cpp \
	     

LIBS = ../SVM/libsvm.a


# include dirs
HMaxFlags = ../../../test/HmaxTest/Hmax-Back-28march/headers
SVMFlags = ../SVM/headers


FLAGS = `pkg-config --cflags opencv` -I${HMaxFlags} -I${SVMFlags}


LINKS = -lpthread `pkg-config --libs opencv` -lga


BASE_OBJ = ${BASE_FILES:.cpp=.o}


all: ${TARGET}


${TARGET}: ${BASE_OBJ} ${LIBS}
	${COMPILER}  	${FLAGS} 	-o 	$@ 	${BASE_OBJ} 	${LIBS} 	${LINKS}
	
clean:
	rm ${BASE_OBJ} *~ ${TARGET}

.cpp.o:
	${COMPILER} 	${FLAGS} 	-c 	$< 	-o 	$@

