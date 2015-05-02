CXX = g++
CFGLAGS = -Wall -Werror -ansi -pedantic
FILEPATH = ./src/ls.cpp
OUTPATH = ./bin/ls

all: 
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)
rshell:
		mkdir -p ,/bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)

clean:
		rm -rf ./bin
