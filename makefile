CXX=g++
CXXFLAGS=-O3 -Wall -pedantic -std=c++11 -I/usr/include/x86_64-linux-gnu/qt5 -fPIC 
LIBS= -lQt5Core -lQt5Gui -lQt5Widgets -lqwt-qt5 -lQt5SerialPort

klient:	main.o MainWindow.o
		$(CXX) -o klient main.o MainWindow.o $(LIBS) $(CXXFLAGS)
main.o:	main.cpp
		$(CXX) -o main.o -c main.cpp $(CXXFLAGS)
MainWindow.o:	MainWindow.cpp MainWindow.moc
		$(CXX) -o MainWindow.o -c MainWindow.cpp $(CXXFLAGS)
MainWindow.moc:	MainWindow.hpp
		moc MainWindow.hpp > MainWindow.moc
		
		
clean:
		rm -f *.o
		rm -f *.moc
		rm -f klient


