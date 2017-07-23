#Kompilator
CXX=g++
#Flagi kompilatora (wysoka optymalizacja, wyświetlanie ostrzeżeń o możliwych błędach, wybór standardu)
CXXFLAGS=-O3 -Wall -pedantic -std=c++11 
#Flagi kompilatora wymagane do prawidłowej kompilacji z użyciem bibliotek Qt
QTFLAGS= -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -fPIC 
#Biblioteki
LIBS= -lQt5Core -lQt5Gui -lQt5Widgets -lqwt-qt5 -lQt5SerialPort

#Konsolidacja programu
klient:	main.o GlowneOkno.o
		$(CXX) -o klient main.o GlowneOkno.o $(LIBS) $(CXXFLAGS) $(QTFLAGS)
#Kompilacja wykonywalnej części pprogramu
main.o:	main.cpp
		$(CXX) -o main.o -c main.cpp $(CXXFLAGS) $(QTFLAGS)
#Kompilacja implementacji głównej klasy programu
GlowneOkno.o:	GlowneOkno.cpp GlowneOkno.moc GlowneOkno.hpp WyborPortu.hpp
		$(CXX) -o GlowneOkno.o -c GlowneOkno.cpp $(CXXFLAGS) $(QTFLAGS)
#Przygotowanie pliku typu .moc dla głównej klasy programu (wymóg prawidłowego działania biblioteki Qt)
GlowneOkno.moc:	GlowneOkno.hpp
		moc GlowneOkno.hpp > GlowneOkno.moc
		
#Usunięcie efektów kompilacji		
clean:
		rm -f *.o
		rm -f *.moc
		rm -f klient


