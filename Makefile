CC=gcc
CXX=g++ 

SRC=./*.c  
BIN=./bin/cpong.exe

GLIB=C:/msys64/mingw64/lib
GINC=C:/msys64/mingw64/include

LIBS=-lraylib 

default: 
	$(CC) $(SRC) -o $(BIN) -I $(GINC) -L $(GLIB) $(LIBS)

run: default 
	$(BIN)