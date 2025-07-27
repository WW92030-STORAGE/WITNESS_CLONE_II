all: compile
	./main

test: compile
	valgrind ./main

compile:
	g++ -g main.cpp -o main -O3