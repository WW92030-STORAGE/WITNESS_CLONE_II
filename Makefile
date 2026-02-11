all: compile
	./main

test: compile
	valgrind ./main

proof:
	g++ -g test.cpp -o test -O3
	./test

compile:
	g++ -g main.cpp -o main -O0

python:
	python3 python/main.py

pythonpaths:
	python3 python/GeneratePaths.py