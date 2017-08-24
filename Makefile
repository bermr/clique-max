all: *.cpp
	g++ -o p *.cpp -std=c++11

run: 
	./p b.clq

