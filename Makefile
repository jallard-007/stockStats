GXX=g++
GXXFLAGS= -std=c++17 -Wall -Wpedantic -Wextra -Wconversion #-Werror

default:
	make all

all: main.o data.o
	$(GXX) -o main main.o data.o $(GXXFLAGS)

main.o: main.cpp
	$(GXX) -c main.cpp $(GXXFLAGS)

data.o: data.cpp data.h
	$(GXX) -c data.cpp $(GXXFLAGS)

clean:
	rm -f main main.o data.o