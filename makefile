HDR = Headers
OBJ = Objects
SRC = Sources

ALL all: build
DEBUG debug: build-debug

CXXFLAGS := -std=c++11 -lm
CXXFLAGS += -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy\
-Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs\
-Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion\
-Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused

build: check
build-debug: CXXFLAGS += -g -g3
build-debug: build

check: $(HDR)/* $(SRC)/*
	$(CXX) $(CXXFLAGS) -I $(HDR) -o check $(SRC)/*.cpp

clean:
	$(RM) check