CXX = g++
CFGLAGS = -Wall -Werror -ansi -pedantic
FILEPATH = ./src/new.cpp
CPPATH = ./src/cp.cpp
RMPATH = ./src/rm.cpp
OUTPATH = ./bin/rshell

all: 
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)
rshell:
		mkdir -p ,/bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)
cp:		
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATHS)
rm: 		
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATHS)

clean:
		rm -rf ./bin

