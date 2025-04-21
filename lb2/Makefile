all: build run

build: main.cpp mgen.cpp
	g++ -o mgen.out mgen.cpp
	g++ main.cpp

run: build
	./a.out

clean:
	rm a.out mgen.out
