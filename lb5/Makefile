all: build run

build: main.cpp
	g++ -std=c++11 main.cpp -lgvc -lcgraph 

run: build
	./a.out

clean:
	rm a.out
