HDR = Headers
OBJ = Objects
SRC = Sources

ALL all: check

check: $(OBJ)/check.o $(OBJ)/parser.o $(OBJ)/array.o $(OBJ)/factor.o
	g++ -std=c++11 -o check $(OBJ)/check.o $(OBJ)/parser.o $(OBJ)/array.o $(OBJ)/factor.o

$(OBJ)/check.o: $(SRC)/check.cpp
	g++ -I ./$(HDR) -c -o $(OBJ)/check.o $(SRC)/check.cpp

$(OBJ)/parser.o: $(HDR)/parser.h $(SRC)/parser.cpp
	g++ -I ./$(HDR) -c -o $(OBJ)/parser.o $(SRC)/parser.cpp

$(OBJ)/array.o: $(HDR)/array.h $(SRC)/array.cpp
	g++ -I ./$(HDR) -c -o $(OBJ)/array.o $(SRC)/array.cpp

$(OBJ)/factor.o: $(HDR)/factor.h $(SRC)/factor.cpp
	g++ -I ./$(HDR) -c -o $(OBJ)/factor.o $(SRC)/factor.cpp