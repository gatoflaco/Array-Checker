HDR = Headers
OBJ = Objects
SRC = Sources

ALL all: check

check: $(OBJ)/check.o $(OBJ)/InputInfo.o $(OBJ)/Array.o $(OBJ)/Factor.o
	g++ -std=c++11 -o check $(OBJ)/check.o $(OBJ)/InputInfo.o $(OBJ)/Array.o $(OBJ)/Factor.o

$(OBJ)/check.o: $(SRC)/check.cpp
	g++ -I ./$(HDR) -c -o $(OBJ)/check.o $(SRC)/check.cpp

$(OBJ)/InputInfo.o: $(HDR)/InputInfo.h $(SRC)/InputInfo.cpp
	g++ -I ./$(HDR) -c -o $(OBJ)/InputInfo.o $(SRC)/InputInfo.cpp

$(OBJ)/Array.o: $(HDR)/Array.h $(SRC)/Array.cpp
	g++ -I ./$(HDR) -c -o $(OBJ)/Array.o $(SRC)/Array.cpp

$(OBJ)/Factor.o: $(HDR)/Factor.h $(SRC)/Factor.cpp
	g++ -I ./$(HDR) -c -o $(OBJ)/Factor.o $(SRC)/Factor.cpp