CXX=g++
CXXFLAGS=-O3 -Wall -pedantic -std=c++11 -Wno-deprecated `pkg-config gtk+-2.0 --cflags`
LIBS=`pkg-config gtk+-2.0 --libs`
klient:	main.o okno.o rs232.o
		$(CXX) -o klient main.o okno.o rs232.o $(LIBS) $(CXXFLAGS)
main.o:	main.cpp
		$(CXX) -o main.o -c main.cpp $(CXXFLAGS)
okno.o:	okno.cpp
		$(CXX) -o okno.o -c okno.cpp $(CXXFLAGS)
rs232.o:	rs232.c
		gcc -o rs232.o -c rs232.c -O3 -Wall -pedantic -std=gnu99 -Wno-deprecated
clean:
		rm -f *.o



