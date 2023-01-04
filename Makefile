findtreasure: main.o
	g++ main.o -o findtreasure

main.o: main.cpp
	g++ -std=c++11 -c main.cpp