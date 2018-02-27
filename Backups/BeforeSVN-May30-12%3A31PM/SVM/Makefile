COMPILER=g++
ARCHIVE = ar

FLAGS = -o2

TARGET = libsvm.a

all : ${TARGET}

FILES = implementations/libsvm.cpp \

OBJ = ${FILES:.cpp=.o}

LIBS = libsvm/svm.o

${TARGET}:  ${OBJ} ${LIBS}
	${ARCHIVE}  rsv  $@   ${OBJ}  ${LIBS}

.cpp.o:
	${COMPILER}  ${FLAGS}   -c  $<  -o $@
	
clean:
	rm ${OBJ} *~ ${TARGET}
