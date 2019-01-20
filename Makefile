CXX = mpic++
CXXFLAGS = -g3 -Wall

LIBS=-lm

Funciones.o: clean Funciones.cpp Funciones.h
	$(CXX) $(CXXFLAGS) -c Funciones.cpp


all:  Funciones.o
	$(CXX) $(CXXFLAGS) -o programa main.cpp  Funciones.o

clean:
	rm -fr *.o a.out programa

.DEFAULT_GOAL := all