#Kompilator
CXX=g++
#Flagi kompilatora (wysoka optymalizacja, wyświetlanie ostrzeżeń o możliwych błędach, wybór standardu)
CXXFLAGS=-O3 -Wall -pedantic -std=c++11 -march=native -mtune=native
#Flagi kompilatora wymagane do prawidłowej kompilacji z użyciem bibliotek Qt
QTFLAGS= -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -fPIC 
#Biblioteki
LIBS= -lQt5Core -lQt5Gui -lQt5Widgets -lqwt-qt5 -lQt5SerialPort

#Konsolidacja programu
klient:	main.o GlowneOkno.o WyborPortu.o TrybProgramowalny.o TrybManualny.o WartosciStale.o
		$(CXX) -o klient main.o GlowneOkno.o WyborPortu.o TrybProgramowalny.o TrybManualny.o WartosciStale.o $(LIBS) $(CXXFLAGS) $(QTFLAGS)
#Kompilacja wykonywalnej części pprogramu
main.o:	main.cpp
		$(CXX) -o main.o -c main.cpp $(CXXFLAGS) $(QTFLAGS)
#Kompilacja implementacji głównej klasy programu
GlowneOkno.o:	GlowneOkno.cpp GlowneOkno.moc GlowneOkno.hpp TypyWyliczeniowe.hpp TrybProgramowalny.hpp TrybManualny.hpp WartosciStale.hpp
		$(CXX) -o GlowneOkno.o -c GlowneOkno.cpp $(CXXFLAGS) $(QTFLAGS)
#Przygotowanie pliku typu .moc dla głównej klasy programu (wymóg prawidłowego działania biblioteki Qt)
GlowneOkno.moc:	GlowneOkno.hpp
		moc GlowneOkno.hpp > GlowneOkno.moc
#Kompilacja implementacji klasy WyborPortu
WyborPortu.o:	WyborPortu.cpp WyborPortu.hpp
		$(CXX) -o WyborPortu.o -c WyborPortu.cpp $(CXXFLAGS) $(QTFLAGS)
		
#Kompilacja implementacji klasy trybu manualnego
TrybManualny.o: TrybManualny.cpp TrybManualny.hpp TrybManualny.moc GlowneOkno.hpp
		$(CXX) -o TrybManualny.o -c TrybManualny.cpp $(CXXFLAGS) $(QTFLAGS)
#Przygotowanie pliku typu .moc dla klasu trybu manualnego (wymóg prawidłowego działania biblioteki Qt)
TrybManualny.moc:	TrybManualny.hpp
		moc TrybManualny.hpp > TrybManualny.moc	
				
#Kompilacja implementacji klasy trybu programowalnego
TrybProgramowalny.o: TrybProgramowalny.cpp TrybProgramowalny.hpp TrybProgramowalny.moc Struktury.hpp TypyWyliczeniowe.hpp GlowneOkno.hpp
		$(CXX) -o TrybProgramowalny.o -c TrybProgramowalny.cpp $(CXXFLAGS) $(QTFLAGS)
#Przygotowanie pliku typu .moc dla klasu trybu programowalnego (wymóg prawidłowego działania biblioteki Qt)
TrybProgramowalny.moc:	TrybProgramowalny.hpp
		moc TrybProgramowalny.hpp > TrybProgramowalny.moc	
		
#Kompilacja implementacji wartości stałych
WartosciStale.o: WartosciStale.cpp WartosciStale.hpp
		$(CXX) -o WartosciStale.o -c WartosciStale.cpp $(CXXFLAGS) $(QTFLAGS)
		
#Usunięcie efektów kompilacji		
clean:
		rm -f *.o
		rm -f *.moc
		rm -f klient


