all: sort

sort: sort.cpp
	g++ -std=c++17 -O3 -o sort sort.cpp
