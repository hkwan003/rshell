CXX = g++
CFGLAGS = -Wall -Werror -ansi -pedantic
FILEPATH = ./src/new.cpp
OUTPATH = ./bin/redirect

all: 
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)
rshell:
		mkdir -p ,/bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)

clean:
		rm -rf ./bin

