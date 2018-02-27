############################################################################################################################################
#	
#	
#
#	
#
#	
#	
#	
#
############################################################################################################################################
COMPILER=g++


TARGET = hmax_genetic


BASE_FILES = main_genetic.cpp \
	     

LIBS = ../SVM/libsvm.a


FLAGS = -o2 `pkg-config --cflags opencv`


LINKS = -lpthread `pkg-config --libs opencv` -lga


BASE_OBJ = ${BASE_FILES:.cpp=.o}


all: ${TARGET}



${TARGET}: ${BASE_OBJ} ${LIBS}
	${COMPILER}  	${FLAGS} 	-o 	$@ 	${BASE_OBJ} 	${LIBS} 	${LINKS}
	

clean:
	rm ${BASE_OBJ} *~ ${TARGET}


.cpp.o:
	${COMPILER} 	${FLAGS} 	-c 	$< 	-o 	$@

