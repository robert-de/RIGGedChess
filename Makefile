CC=g++
TARGET=RiggedChess
FLAGS=-g -Wall

SRC_DIR=./src
OBJ_DIR=./build
OBJ=$(wildcard ./build/*.o)

all: ${OBJ_DIR}/${OBJ}

build: all
	$(MAKE) -C ${SRC_DIR} ${TARGET}
	mv ${SRC_DIR}/${TARGET} ${OBJ_DIR}/${TARGET}
	
run:
	${OBJ_DIR}/${TARGET}

clean:
	$(MAKE) -C ${SRC_DIR} clean
	rm -f ${OBJ_DIR}/RiggedChess
