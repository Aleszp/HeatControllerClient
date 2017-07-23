#Kompilator
CXX=g++
#Flagi kompilatora (wysoka optymalizacja, wyświetlanie ostrzeżeń o możliwych błędach, wybór standardu)
CXXFLAGS=-O3 -Wall -pedantic -std=c++11 
#Flagi kompilatora wymagane do prawidłowej kompilacji z użyciem bibliotek Qt
QTFLAGS= -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -fPIC 
#Biblioteki
LIBS= -lQt5Core -lQt5Gui -lQt5Widgets -lqwt-qt5 -lQt5SerialPort

#Konsolidacja programu
klient:	main.o MainWindow.o
		$(CXX) -o klient main.o MainWindow.o $(LIBS) $(CXXFLAGS) $(QTFLAGS)
#Kompilacja wykonywalnej części pprogramu
main.o:	main.cpp
		$(CXX) -o main.o -c main.cpp $(CXXFLAGS) $(QTFLAGS)
#Kompilacja implementacji głównej klasy programu
MainWindow.o:	MainWindow.cpp MainWindow.moc
		$(CXX) -o MainWindow.o -c MainWindow.cpp $(CXXFLAGS) $(QTFLAGS)
#Przygotowanie pliku typu .moc dla głównej klasy programu (wymóg prawidłowego działania biblioteki Qt)
MainWindow.moc:	MainWindow.hpp
		moc MainWindow.hpp > MainWindow.moc
		
#Usunięcie efektów kompilacji		
clean:
		rm -f *.o
		rm -f *.moc
		rm -f klient


