CXX = g++
CFGLAGS = -Wall -Werror -ansi -pedantic
FILEPATH = ./src/main.cpp
CPPATH = ./src/cp.cpp
RMPATH = ./src/rm.cpp
LSPATH = ./src/ls.cpp
MVPATH = ./src/mv.cpp
OUTPATH = ./bin/rshell
all: 
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)
rshell:
		mkdir -p ,/bin
		$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)
cp:		
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(CPPATH) -o $(OUTPATH)
rm: 		
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(RMPATH) -o $(OUTPATH)
ls: 	
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(LSPATH) -o $(OUTPATH)
mv:
		mkdir -p ./bin
		$(CXX) $(CFLAGS) $(MVPATH) -o $(OUTPATH)

clean:
		rm -rf ./bin

