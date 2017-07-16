CXX=g++
CXXFLAGS=-O3 -Wall -pedantic -std=c++11 -I/usr/include/x86_64-linux-gnu/qt5 -fPIC 
LIBS= -lQt5Core -lQt5Gui -lQt5Widgets -lqwt-qt5 -lQt5SerialPort
klient:	main.o
		$(CXX) -o klient main.o $(LIBS) $(CXXFLAGS)
main.o:	main.cpp
		$(CXX) -o main.o -c main.cpp $(CXXFLAGS)
clean:
		rm -f *.o



