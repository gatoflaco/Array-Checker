HDR = Headers
OBJ = Objects
SRC = Sources

ALL all: check

check: $(OBJ)/check.o $(OBJ)/InputInfo.o
	g++ -std=c++11 -o check $(OBJ)/check.o $(OBJ)/InputInfo.o

$(OBJ)/check.o: $(SRC)/check.cpp
	g++ -I ./$(HDR) -c -o $(OBJ)/check.o $(SRC)/check.cpp

$(OBJ)/InputInfo.o: $(SRC)/InputInfo.cpp
	g++ -I ./$(HDR) -c -o $(OBJ)/InputInfo.o $(SRC)/InputInfo.cpp

#$(OBJ)/Graph.o: $(HDR)/Graph.h $(SRC)/Graph.cpp
#	g++ -I ./$(HDR) -c -o $(OBJ)/Graph.o $(SRC)/Graph.cpp