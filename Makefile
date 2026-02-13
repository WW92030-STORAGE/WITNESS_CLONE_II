all: compile
	./main

test: compile
	valgrind ./main

proof:
	g++ exhaustivesearch.cpp -o exhaustivesearch -O3
	./exhaustivesearch

compile:
	g++ main.cpp -o main -O3

python:
	python3 python/main.py

pythonpaths:
	python3 python/GeneratePaths.py


preprofile:
	g++ main.cpp -o main -O1 -pg -w

doprofile:
	./main
	gprof main > analysis.txt

profile: preprofile doprofile