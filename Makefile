CXX = g++
CFGLAGS = -Wall -Werror -ansi -pedantic
FILEPATH = ./src/main.cpp
OUTPATH = ./bin/rshell

all: 
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)
rshell:
		mkdir -p ,/bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)

clean:
		rm -rf ./bin
